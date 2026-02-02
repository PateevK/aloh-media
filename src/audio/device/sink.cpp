#include "audio/device/device.hpp"
#include "device.hpp"

#include "audio/Utils/AudioTypes.hpp"
#include "audio/Utils/AudioTypesImpl.hpp"

#include "miniaudio.h"

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

} // audio

} // alo