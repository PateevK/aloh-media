#pragma once

#include "device.hpp"

#include <optional>

namespace alo{

namespace audio{
    // ============ Device<DeviceType::SRC> (Capture) ============


    template<>
    Device<DeviceType::SRC>::Device(device_info_ptr info,  device_id_t id);

    template<>
    Device<DeviceType::SRC>::~Device();

    template<>
    void Device<DeviceType::SRC>::cb(device_cb_t _cb);

    template<>
    void Device<DeviceType::SRC>::_data_callback_c(ma_device* pDevice, void* pOutput, const void* pInput, uint32_t frameCount);

    template<>  
    std::optional<err_t> Device<DeviceType::SRC>::init(const context_ptr& con);

    template<>
    std::optional<err_t> Device<DeviceType::SRC>::start();

    template<>
    std::optional<err_t> Device<DeviceType::SRC>::stop();

} // audio

} // alo