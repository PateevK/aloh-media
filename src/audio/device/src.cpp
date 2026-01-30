#include "device.hpp"

#include "audio/Utils/AudioTypes.hpp"
#include "audio/Utils/AudioTypesImpl.hpp"

#include "miniaudio.h"

#include <optional>

namespace alo{

namespace audio{
    // ============ Device<DeviceType::SRC> (Playback) ============

    template<>
    Device<DeviceType::SRC>::Device(){
        _device = device::make();
    }

    template<>
    Device<DeviceType::SRC>::~Device(){
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

    template<>  
    std::optional<err_t> Device<DeviceType::SRC>::init(){
        auto conf = ma_device_config_init(ma_device_type_playback);

        conf.playback.pDeviceID = NULL;
        conf.playback.format    = ma_format_f32;
        conf.playback.channels  = 2;
            
        conf.dataCallback = _data_callback_c;
        conf.pUserData    = this;
        
        auto result = ma_device_init(NULL, &conf, _device->get());

        if(result != MA_SUCCESS){
            return result;
        }

        return std::nullopt;
    }

    template<>
    std::optional<err_t> Device<DeviceType::SRC>::start(){
        auto result = ma_device_start(_device->get());
        
        if(result != MA_SUCCESS){
            _device.reset();
            return result;
        }

        return std::nullopt;
    }

    template<>
    std::optional<err_t> Device<DeviceType::SRC>::stop(){
        auto result = ma_device_stop(_device->get());
        if(result != MA_SUCCESS){
            _device.reset();
            return result;
        }

        return std::nullopt;
    }

} // audio

} // alo