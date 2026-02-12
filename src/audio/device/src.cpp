#include "audio/Utils/ring_buffer.hpp"
#include "device.hpp"

#include "audio/Utils/AudioTypes.hpp"
#include "audio/Utils/AudioTypesImpl.hpp"

#include "miniaudio.h"
#include "spdlog/spdlog.h"
#include <memory>
#include <stdexcept>

namespace alo{

namespace audio{
    // ============ Device<DeviceType::SRC> (Capture) ============

    template<>
    Device<DeviceType::SRC>::Device(device_info_ptr info,  device_id_t id, context_ref context)
    : _device(device::make()), _info(std::move(info)), _id(std::move(id)), _context(context) {

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

        new (reinterpret_cast<void*>(std::addressof(_rb))) utils::RingBuffer<utils::RingBufferType::PCM>(std::move(res.value()));
    }

    template<>
    void Device<DeviceType::SRC>::cb(device_cb_t _cb){
        _data_cb = _cb;  
    }

    template<>
    void Device<DeviceType::SRC>::_data_callback_c(ma_device* pDevice, void* pOutput, const void* pInput, uint32_t frameCount){
        Device<DeviceType::SRC>* self = static_cast<Device<DeviceType::SRC>*>(pDevice->pUserData);

        if(self && self->_data_cb){
            self->_data_cb(self, pOutput, pInput, frameCount);
        }
    }

} // audio

} // alo