#pragma once

#include <audio/Utils/AudioTypes.hpp>

#include <optional>
#include <string>

// forward
struct ma_device;
struct ma_device_config;

namespace alo::audio{

using device_id_t = std::string;
using err_t = int;
enum class DeviceType { SRC, SINK };

struct DeviceConf{
    ma_device_config* _conf;
};

template<DeviceType Type>    
class Device {
public:
    // Type-safe callback signature
    using device_cb_t = std::function<void(Device<Type>* device, void* pOutput, const void* pInput, uint32_t frameCount)>;

    Device();
    ~Device();

    void cb(device_cb_t cb);
    
    std::optional<err_t> init(); 
    std::optional<err_t> start();
    std::optional<err_t> stop();

private:
    device_ptr _device = nullptr;
    device_cb_t _data_cb;

    static void _data_callback_c(ma_device* pDevice, void* pOutput, const void* pInput, uint32_t frameCount);
};

} // alo::audio

#include "./src.tpp" // IWYU pragma: keep
#include "./sink.tpp" // IWYU pragma: keep