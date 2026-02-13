#pragma once

#include <spdlog/spdlog.h>
#include <print>

#include <audio/device/device.hpp>
#include <audio/engine/device_m.hpp>
#include <audio/node/node.hpp>

namespace alo::audio::node{

namespace aa = alo::audio;

class Src{
    aa::device_handle_t<aa::DeviceType::SRC> _device{};
    Node* _other_node = nullptr;

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
    
    // Pull audio data from device's ring buffer
    // Returns the number of frames actually read
    uint32_t pull(float* data, uint32_t frame_count) {
        if (!_device || !data || frame_count == 0) {
            return 0;
        }
        return _device->pull(data, frame_count);
    }

    // Returns frames available for reading from device
    uint32_t available() const {
        if (!_device) {
            return 0;
        }
        return _device->available();
    }

    // Device format info
    uint32_t channels() const { return _device ? _device->channels() : 0; }
    uint32_t sample_rate() const { return _device ? _device->sample_rate() : 0; }
    
    void build() {
         spdlog::debug("{} | {} | rate({}), ch({})", FUNC_SIG, _device->id(), _device->sample_rate(), _device->channels());

        auto err = _device->init();
        if(err){
            spdlog::error("{} | err = {}", FUNC_SIG, err.value());
            return;
        }
        // Device handles buffering internally - no callback setup needed
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