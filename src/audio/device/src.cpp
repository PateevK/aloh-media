#include "device.hpp"

#include "audio/Utils/AudioTypes.hpp"
#include "audio/Utils/AudioTypesImpl.hpp"

#include "miniaudio.h"

namespace alo{

namespace audio{
    // ============ Device<DeviceType::SRC> (Capture) ============

    template<>
    Device<DeviceType::SRC>::Device(device_info_ptr info,  device_id_t id, context_ref context)
    : _device(device::make()), _info(std::move(info)), _id(std::move(id)), _context(context) {
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