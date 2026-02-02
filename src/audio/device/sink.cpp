#include "audio/device/device.hpp"
#include "device.hpp"

#include "audio/Utils/AudioTypes.hpp"
#include "audio/Utils/AudioTypesImpl.hpp"

#include "miniaudio.h"
#include "spdlog/spdlog.h"

#include <optional>
#include <print>

namespace alo{

namespace audio{
    // ============ Device<DeviceType::SINK> (Playback) ============

    template<>
    Device<DeviceType::SINK>::Device(device_info_ptr info, device_id_t id) 
        : _device(device::make()), _info(std::move(info)), _id(std::move(id)){
        }

    template<>
    Device<DeviceType::SINK>::~Device(){
    }

    template<>
    void Device<DeviceType::SINK>::cb(device_cb_t _cb){
        _data_cb = _cb;  
    }

    template<>
    void Device<DeviceType::SINK>::_data_callback_c(ma_device* pDevice, void* pOutput, const void* pInput, uint32_t frameCount){
        Device<DeviceType::SINK>* self = static_cast<Device<DeviceType::SINK>*>(pDevice->pUserData);

        if(self && self->_data_cb){
            self->_data_cb(self, pOutput, pInput, frameCount);
        }
    }

    template<>  
    std::optional<err_t> Device<DeviceType::SINK>::init(){
        auto conf = ma_device_config_init(ma_device_type_playback);


        if( !_info ){
            spdlog::error("Device<SINK>::init : _info is NULL");
            return 1;
        }

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
    std::optional<err_t> Device<DeviceType::SINK>::start(){
        auto result = ma_device_start(_device->get());
        
        if(result != MA_SUCCESS){
            _device.reset();
            return result;
        }

        return std::nullopt;
    }

    template<>
    std::optional<err_t> Device<DeviceType::SINK>::stop(){
        auto result = ma_device_stop(_device->get());
        if(result != MA_SUCCESS){
            _device.reset();
            return result;
        }

        return std::nullopt;
    }

} // audio

} // alo