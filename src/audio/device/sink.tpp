#pragma once

#include "device.hpp"

#include <optional>

namespace alo{

namespace audio{
    // ============ Device<DeviceType::SINK> (Playback) ============

    template<>
    Device<DeviceType::SINK>::Device();

    template<>
    Device<DeviceType::SINK>::~Device();

    template<>
    void Device<DeviceType::SINK>::cb(device_cb_t _cb);

    template<>
    void Device<DeviceType::SINK>::_data_callback_c(ma_device* pDevice, void* pOutput, const void* pInput, uint32_t frameCount);

    template<>  
    std::optional<err_t> Device<DeviceType::SINK>::init();

    template<>
    std::optional<err_t> Device<DeviceType::SINK>::start();

    template<>
    std::optional<err_t> Device<DeviceType::SINK>::stop();

} // audio

} // alo