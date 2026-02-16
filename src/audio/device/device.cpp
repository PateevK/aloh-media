#include "device.hpp"
#include "audio/device/device.hpp"
#include "audio/Utils/AudioTypes.hpp"
#include "audio/Utils/AudioTypesImpl.hpp"

#include <cstring>
#include <memory>
#include <optional>
#include <utility>
#include <spdlog/spdlog.h>

namespace alo::audio{
    
template<DeviceType type>
std::shared_ptr<Device<type>> Device<type>::make(device_info_ptr info,  device_id_t id, context_ref context){
    return std::shared_ptr<Device<type>>(new Device<type>(std::move(info), std::move(id), context));
};

template<DeviceType type>
Device<type>::Device(device_info_ptr info,  device_id_t id, context_ref context) 
    : _device(device::make()), _info(std::move(info)), _id(std::move(id)), _context(context) 
{
    const auto& num_formats = _info->get()->nativeDataFormatCount;
    
    // Use native formats if available, otherwise use sensible defaults
    uint32_t num_channels_def = 2;      // Default: stereo
    uint32_t sample_rate_def = 48000;   // Default: 48kHz
    
    for(int i{0}; i < num_formats; i++){
        const auto& formats = _info->get()->nativeDataFormats[i];
        spdlog::debug("{} | {} | DataFormat = ({}) | buffer for device: format = {}, sample_rate = {}, num_channels = {}", 
        FUNC_SIG, _info->get()->name , i, std::to_underlying(formats.format), formats.sampleRate, formats.channels);
    }

    if (num_formats >= 1) {
        const auto& formats = _info->get()->nativeDataFormats[0];
        _num_channels = formats.channels > 0 ? formats.channels : num_channels_def;
        _sample_rate = formats.sampleRate > 0 ? formats.sampleRate : sample_rate_def;
    } else {
        spdlog::warn("{} | No native formats available, using defaults ({}ch, {}Hz)", 
            FUNC_SIG, _num_channels, _sample_rate);
    }

    const uint32_t size_in_frames = _sample_rate / 2;  // 500ms buffer
    spdlog::info("{} | {} | DataFormatCount ({}) | buffer for device: size_in_frames = {}, sample_rate = {}, num_channels = {}", 
        FUNC_SIG, _info->get()->name , num_formats, size_in_frames, _sample_rate, _num_channels);

    auto res = utils::RingBuffer<utils::RingBufferType::PCM>::make(_num_channels, size_in_frames);
    if(!res.has_value()){
        spdlog::error("{} | Failed to create ring buffer", FUNC_SIG);
        throw std::runtime_error(std::format("{} | Failed to create ring buffer", FUNC_SIG));
    }

    _rb = std::move(res.value());
}

template<DeviceType type>
Device<type>::~Device(){
    auto res = stop();
    if(res){
        spdlog::warn("{} | device is not stopped, forcing unInit()", FUNC_SIG);
    }
    unInit();
}

template<DeviceType type>
void Device<type>::cb(device_cb_t cb) {
    _data_cb = cb;  
}

template<DeviceType type>
void Device<type>::_data_callback_c(ma_device* pDevice, void* pOutput, const void* pInput, uint32_t frameCount){
    Device<type>* self = static_cast<Device<type>*>(pDevice->pUserData);
    if (!self) return;

    if constexpr (type == DeviceType::SRC) {
        // Capture device: write incoming audio to ring buffer
        if (pInput && frameCount > 0) {
            const float* input_data = static_cast<const float*>(pInput);
            self->_rb.write(input_data, frameCount);
        }
        // External callback after capture (for monitoring/additional processing)
        if (self->_data_cb) {
            self->_data_cb(self, pOutput, pInput, frameCount);
        }
    } else {
        // Playback device: external callback FIRST to fill buffer
        if (self->_data_cb) {
            self->_data_cb(self, pOutput, pInput, frameCount);
        }
        // Then read from ring buffer to output
        if (pOutput && frameCount > 0) {
            float* output_data = static_cast<float*>(pOutput);
            uint32_t frames_read = self->_rb.read(output_data, frameCount);
            
            // Zero-fill if we didn't get enough data (avoid noise)
            if (frames_read < frameCount) {
                uint32_t remaining_samples = (frameCount - frames_read) * self->_num_channels;
                std::memset(output_data + (frames_read * self->_num_channels), 0, 
                           remaining_samples * sizeof(float));
            }
        }
    }
}

template<DeviceType type>
uint32_t Device<type>::pull(float* data, uint32_t frame_count) {
    if (!data || frame_count == 0) return 0;
    return _rb.read(data, frame_count);
}

template<DeviceType type>
uint32_t Device<type>::write(const float* data, uint32_t frame_count) {
    if (!data || frame_count == 0) return 0;
    return _rb.write(data, frame_count);
}

template<DeviceType type>
uint32_t Device<type>::available() const {
    return _rb.available_read();
}

template<DeviceType type>
device_id_t Device<type>::id() const {
    return _id;
}

template<DeviceType type>
void Device<type>::id(device_id_t id){
    _id = std::move(id);
}

template<DeviceType type>
bool Device<type>::is_init() const {
    return _is_init;
}

template<DeviceType type>
bool Device<type>::is_started() const {
    return _is_started;
}

template<DeviceType type>
std::optional<err_t> Device<type>::init(){
    if(is_init()){
        return std::nullopt;
    }

    if( !_info ){
        spdlog::error("{} | _info is NULL", FUNC_SIG);
        return 1;
    }

    auto conf = ma_device_config_init(
        type == DeviceType::SRC ? ma_device_type_capture : ma_device_type_playback
    );

    if constexpr (type == DeviceType::SRC) {
        conf.capture.pDeviceID = &_info->get()->id;
        conf.capture.format    = ma_format_f32;
        conf.capture.channels  = _num_channels;
        conf.sampleRate  = _sample_rate;
    } else {
        conf.playback.pDeviceID = &_info->get()->id;
        conf.playback.format    = ma_format_f32;
        conf.playback.channels  = _num_channels;
        conf.sampleRate  = _sample_rate;
    }
        
    conf.dataCallback = _data_callback_c;
    conf.pUserData    = this;
    
    if(!_context){
        spdlog::error("{} | if(!_context)", FUNC_SIG);
        return 2;
    }

    auto result = ma_device_init(_context->get(), &conf, _device->get());
    if(result != MA_SUCCESS){
        spdlog::error("{} | result != MA_SUCCESS", FUNC_SIG);
        return result;
    }

    _is_init = true;
    return std::nullopt;
}

template<DeviceType type>
 std::optional<err_t> Device<type>::start(){
    
    if(!is_init()){
        spdlog::error(" {} | init device first", FUNC_SIG);
        return 1;
    } 

    auto result = ma_device_start(_device->get());
     
     if(result != MA_SUCCESS){
         _device.reset();
         return result;
     }

     _is_started = true;
     return std::nullopt;
 }
 
 template<DeviceType type>
 std::optional<err_t> Device<type>::stop(){
     auto result = ma_device_stop(_device->get());
     if(result != MA_SUCCESS){
         _device.reset();
         return result;
     }

     _is_started = false;
     return std::nullopt;
 }

template<DeviceType type>
void Device<type>::unInit() {
    if(!is_init()){
        return;
    }
    if(is_started()){
        stop();
    }
    ma_device_uninit(_device->get());
}
 

template class Device<DeviceType::SRC>;
template class Device<DeviceType::SINK>;

} // alo::audio