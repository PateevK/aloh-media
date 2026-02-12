#pragma once

#include "spdlog/spdlog.h"
#include <cstdint>
#include <cstring>
#include <print>

#include <audio/device/device.hpp>
#include <audio/engine/device_m.hpp>
#include <audio/node/node.hpp>


namespace alo::audio::node{

namespace aa = alo::audio;

class Sink{
    aa::device_handle_t<aa::DeviceType::SINK> _device{};
    // Sink is terminal - no next node needed
    Node* _other_node = nullptr;
    // Save log's pants.
    bool _hot_path_log_b{false};
public:
    Sink(aa::device_handle_t<aa::DeviceType::SINK> device) : _device(device) {}

    void connect(Node* node) {
        spdlog::debug("Sink::connect (terminal node)");
        if(node == nullptr){
            spdlog::warn("{} | if(next == nullptr)", FUNC_SIG);
        }

        _other_node = node;
    }

    void push() const {
        std::println("Sink::push ???");
    }
    
    // Pull audio data from connected node into the provided buffer
    uint32_t pull(float* data, uint32_t frame_count) {
        if (_other_node == nullptr) {
            if (!_hot_path_log_b) {
                spdlog::error("{} | if(_other_node == nullptr)", FUNC_SIG);
                _hot_path_log_b = true;
            }
            return 0;
        }
        return _other_node->pull(data, frame_count);
    }

    void build(){
        spdlog::debug("{}", FUNC_SIG);
        auto err = _device->init();
        if(err){
            spdlog::error("{} | err = {}", FUNC_SIG, err.value());
            return;
        }

        auto cb = [this](Device<DeviceType::SINK>* device, void* pOutput, const void* pInput, uint32_t frameCount){
            float* output_data = static_cast<float*>(pOutput);
            uint32_t frames_read = this->pull(output_data, frameCount);
            
            // If we didn't get enough data, zero-fill the rest to avoid noise
            if (frames_read < frameCount) {
                uint32_t channels = 2; // TODO: get from device config
                std::memset(output_data + (frames_read * channels), 0, 
                           (frameCount - frames_read) * channels * sizeof(float));
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