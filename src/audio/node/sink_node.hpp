#pragma once

#include "spdlog/spdlog.h"
#include <cstdint>
#include <print>
#include <vector>

#include <audio/device/device.hpp>
#include <audio/engine/device_m.hpp>
#include <audio/node/node.hpp>


namespace alo::audio::node{

namespace aa = alo::audio;

class Sink{
    aa::device_handle_t<aa::DeviceType::SINK> _device{};
    Node* _upstream = nullptr;
    // Save log's pants.
    bool _hot_path_log_b{false};
    // Temp buffer for pulling from upstream before writing to device
    std::vector<float> _pull_buffer;

public:
    Sink(aa::device_handle_t<aa::DeviceType::SINK> device) : _device(device) {}

    void connect(Node* upstream, Node* downstream) {
        spdlog::debug("Sink::connect (terminal node)");
        if (upstream != nullptr)  _upstream = upstream;
        (void)downstream;
    }

    void push() const {
        std::println("Sink::push ???");
    }
    
    // Pull audio data from connected node into the provided buffer
    uint32_t pull(float* data, uint32_t frame_count) {
        if (_upstream == nullptr) {
            if (!_hot_path_log_b) {
                spdlog::error("{} | if(_upstream == nullptr)", FUNC_SIG);
                _hot_path_log_b = true;
            }
            return 0;
        }
        return _upstream->pull(data, frame_count);
    }

    // Device format info
    uint32_t channels() const { return _device ? _device->channels() : 0; }
    uint32_t sample_rate() const { return _device ? _device->sample_rate() : 0; }

    void build(){
        spdlog::debug("{}", FUNC_SIG);
        auto err = _device->init();
        if(err){
            spdlog::error("{} | err = {}", FUNC_SIG, err.value());
            return;
        }

        spdlog::debug("{} | {} | rate({}), ch({})", FUNC_SIG, _device->id(), _device->sample_rate(), _device->channels());
        // Pre-allocate pull buffer (enough for typical callback sizes)
        const uint32_t max_frames = 4096;
        _pull_buffer.resize(max_frames * _device->channels());

        // Callback: pull from upstream and write to device's buffer
        auto cb = [this](Device<DeviceType::SINK>* device, void* pOutput, const void* pInput, uint32_t frameCount){
            if (_upstream == nullptr) {
                return;
            }
            
            // Ensure buffer is large enough
            const uint32_t samples_needed = frameCount * device->channels();
            if (_pull_buffer.size() < samples_needed) {
                _pull_buffer.resize(samples_needed);
            }
            
            // Pull from upstream into temp buffer
            uint32_t frames_read = this->pull(_pull_buffer.data(), frameCount);
            
            // Write to device's ring buffer (device will read from it after this callback)
            if (frames_read > 0) {
                device->write(_pull_buffer.data(), frames_read);
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

    void stop(){
        
    }
};

} // namespace aloh::media::node