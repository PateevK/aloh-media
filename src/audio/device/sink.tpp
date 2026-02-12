#pragma once

#include "audio/device/device.hpp"
#include "device.hpp"

namespace alo{

namespace audio{
    // ============ Device<DeviceType::SINK> (Playback) ============

    template<>
    Device<DeviceType::SINK>::Device(device_info_ptr info, device_id_t id, context_ref context);

    template<>
    void Device<DeviceType::SINK>::cb(device_cb_t _cb);

    template<>
    void Device<DeviceType::SINK>::_data_callback_c(ma_device* pDevice, void* pOutput, const void* pInput, uint32_t frameCount);

} // audio

} // alo