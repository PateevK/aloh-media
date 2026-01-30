#pragma once

#include <audio/Utils/AudioTypes.hpp>

#include <optional>
#include <string>

// forward
struct ma_device;

namespace alo::audio{

using device_id_t = std::string;
using err_t = int;
enum class DeviceType { SRC, SINK };

template<DeviceType Type>    
class Device {
public:
    // Type-safe callback signature
    using device_cb_t = std::function<void(Device<Type>* device, void* pOutput, const void* pInput, uint32_t frameCount)>;

    Device(device_info_ptr info);
    Device(Device&&) = default;
    Device& operator=(Device&&) = default;
    
    // Prevent copying (unique_ptr members make this non-copyable anyway)
    Device(const Device&) = delete;
    Device& operator=(const Device&) = delete;
    ~Device();

    void cb(device_cb_t cb);
    
    std::optional<err_t> init(); 
    std::optional<err_t> start();
    std::optional<err_t> stop();

private:
    device_ptr _device = nullptr;
    device_cb_t _data_cb;
    device_info_ptr _info{};

    static void _data_callback_c(ma_device* pDevice, void* pOutput, const void* pInput, uint32_t frameCount);
};

} // alo::audio

#include "./src.tpp" // IWYU pragma: keep
#include "./sink.tpp" // IWYU pragma: keep