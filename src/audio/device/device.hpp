#pragma once

#include <audio/Utils/AudioTypes.hpp>
#include <audio/Utils/ring_buffer.hpp>

#include <cstdint>
#include <optional>
#include <string>
#include <functional>

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

    static std::shared_ptr<Device<Type>> make(device_info_ptr info,  device_id_t id, context_ref context);

    ~Device();
    
    void cb(device_cb_t cb);

    device_id_t id() const;
    void id(device_id_t id);
    bool is_init() const;
    bool is_started() const;
    uint32_t channels() const{
        return _num_channels;
    }
    uint32_t sample_rate() const{
        return _sample_rate;
    }
    
    std::optional<err_t> init(); 
    void unInit();
    std::optional<err_t> start();
    std::optional<err_t> stop();

    // Ring buffer access - SRC devices write internally, SINK devices read internally
    // For SRC: pull() reads captured audio from buffer
    // For SINK: write() queues audio for playback
    uint32_t pull(float* data, uint32_t frame_count);
    uint32_t write(const float* data, uint32_t frame_count);
    uint32_t available() const;

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
    uint32_t _num_channels;
    uint32_t _sample_rate;

    static void _data_callback_c(ma_device* pDevice, void* pOutput, const void* pInput, uint32_t frameCount);
};

} // alo::audio