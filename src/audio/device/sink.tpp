#pragma once

#include "audio/device/device.hpp"
#include "device.hpp"

#include <optional>

namespace alo{

namespace audio{
    // ============ Device<DeviceType::SINK> (Playback) ============

    template<>
    Device<DeviceType::SINK>::Device(device_info_ptr info, device_id_t id);

    template<>
    Device<DeviceType::SINK>::~Device();

    template<>
    void Device<DeviceType::SINK>::cb(device_cb_t _cb);

    template<>
    void Device<DeviceType::SINK>::_data_callback_c(ma_device* pDevice, void* pOutput, const void* pInput, uint32_t frameCount);

    template<>  
    std::optional<err_t> Device<DeviceType::SINK>::init(const context_ptr& con);

    template<>
    std::optional<err_t> Device<DeviceType::SINK>::start();

    template<>
    std::optional<err_t> Device<DeviceType::SINK>::stop();

} // audio

} // alo