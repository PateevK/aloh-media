#pragma once

#include <spdlog/spdlog.h>
#include <print>

#include <audio/device/device.hpp>
#include <audio/engine/device_m.hpp>
#include <audio/node/node.hpp>
#include <audio/Utils/ring_buffer.hpp>

namespace alo::audio::node{

namespace aa = alo::audio;

class Src{
    aa::device_handle_t<aa::DeviceType::SRC> _device{};
    Node* _other_node = nullptr;
    bool _hot_path_log_b{false};
    std::optional<aa::utils::RingBuffer<aa::utils::RingBufferType::PCM>> _ring_buffer;
    uint32_t _channels{2};
    uint32_t _buffer_size_frames{4096};

public:
    Src(aa::device_handle_t<aa::DeviceType::SRC> device) : _device(device) {}
        
    void connect(Node* other) {
        spdlog::debug("Src::connect");
        if(other == nullptr){
            spdlog::warn("{} | if(other == nullptr)", FUNC_SIG);
        }
        _other_node = other;
    }

    void push() {
        std::println("Src::push");
    }
    
    // Pull audio data from the ring buffer
    // Returns the number of frames actually read
    uint32_t pull(float* data, uint32_t frame_count) {
        if (!_ring_buffer.has_value() || !data || frame_count == 0) {
            return 0;
        }
        return _ring_buffer->read(data, frame_count);
    }

    // Returns frames available for reading
    uint32_t available() const {
        if (!_ring_buffer.has_value()) {
            return 0;
        }
        return _ring_buffer->available_read();
    }
    
    void build() {
        spdlog::debug("{}", FUNC_SIG);
        
        // Initialize ring buffer
        _ring_buffer = aa::utils::RingBuffer<aa::utils::RingBufferType::PCM>::make(_channels, _buffer_size_frames);
        if (!_ring_buffer.has_value()) {
            spdlog::error("{} | Failed to create ring buffer", FUNC_SIG);
            return;
        }

        auto err = _device->init();
        if(err){
            spdlog::error("{} | err = {}", FUNC_SIG, err.value());
            return;
        }

        auto cb = [this](Device<DeviceType::SRC>* device, void* pOutput, const void* pInput, uint32_t frameCount){
            if (!_ring_buffer.has_value() || pInput == nullptr || frameCount == 0) {
                return;
            }
            
            const float* input_data = static_cast<const float*>(pInput);
            uint32_t frames_written = _ring_buffer->write(input_data, frameCount);
            
            if (_hot_path_log_b && frames_written < frameCount) {
                spdlog::warn("Src::cb | Ring buffer overflow: wrote {}/{} frames", frames_written, frameCount);
            }
        };

        _device->cb(cb);
    }
    
    void start() {
        spdlog::debug("{}", FUNC_SIG);
        auto err = _device->start();
        if(err){
            spdlog::error("{} | err = {}", FUNC_SIG, err.value());
        }
    }
};
 
} // namespace aloh::media::node