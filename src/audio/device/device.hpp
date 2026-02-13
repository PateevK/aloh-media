#pragma once

#include <audio/Utils/AudioTypes.hpp>
#include <audio/Utils/ring_buffer.hpp>

#include <cstdint>
#include <optional>
#include <functional>
#include <print>

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

    
    static Device<Type> make(device_info_ptr info,  device_id_t id, context_ref context);
    
    ~Device();
    
    void cb(device_cb_t cb);

    void do_shit() const {
        std::println("some shit");
    }

    device_id_t id() const;
    void id(device_id_t id);
    bool is_init() const;
    bool is_started() const;
    
    std::optional<err_t> init(); 
    void unInit();
    std::optional<err_t> start();
    std::optional<err_t> stop();

private:

    Device(device_info_ptr info, device_id_t id, context_ref context);
    Device(Device&&) = default;
    Device& operator=(Device&&) = default;
    
    // Prevent copying (unique_ptr members make this non-copyable anyway)
    Device(const Device&) = delete;
    Device& operator=(const Device&) = delete;

    bool _is_init = false;
    bool _is_started = false;
    device_id_t _id{};
    device_ptr _device = nullptr;
    device_cb_t _data_cb;
    device_info_ptr _info{};
    context_ref _context{};
    utils::RingBuffer<utils::RingBufferType::PCM> _rb;

    static void _data_callback_c(ma_device* pDevice, void* pOutput, const void* pInput, uint32_t frameCount);
};

} // alo::audio