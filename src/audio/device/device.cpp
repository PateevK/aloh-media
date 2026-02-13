#include "device.hpp"
#include "audio/device/device.hpp"
#include "audio/Utils/AudioTypes.hpp"
#include "audio/Utils/AudioTypesImpl.hpp"

#include <optional>
#include <spdlog/spdlog.h>

namespace alo::audio{

template<DeviceType type>
Device<type>::Device(device_info_ptr info,  device_id_t id, context_ref context) 
    : _device(device::make()), _info(std::move(info)), _id(std::move(id)), _context(context) 
{
    const auto& num_formats = _info->get()->nativeDataFormatCount;
    
    // Use native formats if available, otherwise use sensible defaults
    uint32_t num_channels = 2;      // Default: stereo
    uint32_t sample_rate = 48000;   // Default: 48kHz
    
    if (num_formats >= 1) {
        const auto& formats = _info->get()->nativeDataFormats[0];
        num_channels = formats.channels > 0 ? formats.channels : num_channels;
        sample_rate = formats.sampleRate > 0 ? formats.sampleRate : sample_rate;
    } else {
        spdlog::warn("{} | No native formats available, using defaults ({}ch, {}Hz)", 
            FUNC_SIG, num_channels, sample_rate);
    }

    const uint32_t size_in_frames = sample_rate / 2;  // 500ms buffer
    spdlog::info("{} | buffer for device: size_in_frames = {}, sample_rate = {}, num_channels = {}", 
        FUNC_SIG, size_in_frames, sample_rate, num_channels);

    auto res = utils::RingBuffer<utils::RingBufferType::PCM>::make(num_channels, size_in_frames);
    if(!res.has_value()){
        spdlog::error("{} | Failed to create ring buffer", FUNC_SIG);
        throw std::runtime_error(std::format("{} | Failed to create ring buffer", FUNC_SIG));
    }

    // holly molly
    new (reinterpret_cast<void*>(std::addressof(_rb))) utils::RingBuffer<utils::RingBufferType::PCM>(std::move(res.value()));
}

template<DeviceType type>
Device<type>::~Device(){
    auto res = stop();
    if(res){
        spdlog::error("{} | device is not stopped, forcing unInit()", FUNC_SIG);
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

    if(self && self->_data_cb){
        self->_data_cb(self, pOutput, pInput, frameCount);
    }
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
        conf.capture.channels  = 2;
    } else {
        conf.playback.pDeviceID = &_info->get()->id;
        conf.playback.format    = ma_format_f32;
        conf.playback.channels  = 2;
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