#pragma once

#include "spdlog/spdlog.h"
#include <print>

#include <audio/device/device.hpp>
#include <audio/engine/device_m.hpp>
#include <audio/node/node.hpp>


namespace alo::audio::node{

namespace aa = alo::audio;

class Sink{
    aa::device_handle_t<aa::DeviceType::SINK> _device{};
    // Sink is terminal - no next node needed
    Node* _prev_node = nullptr;
public:
    Sink(aa::device_handle_t<aa::DeviceType::SINK> device) : _device(device) {}

    void connect(Node* next) {
        if(next == nullptr){
            spdlog::warn("{} | if(next == nullptr)", FUNC_SIG);
        }
        _prev_node = next;
        spdlog::debug("Sink::connect (terminal node)");
    }

    void push() const {
        std::println("Sink::push");
    }
    
    void pull() const {
        std::println("Sink::pull");
    }

    void build() const {
        auto err = _device->init();
        if(err){
            spdlog::error("{} | err = {}", FUNC_SIG, err.value());
        }
        spdlog::debug("{}", FUNC_SIG);
    }
    
    void start() {
        auto err = _device->start();
        if(err){
            spdlog::error("{} | err = {}", FUNC_SIG, err.value());
        }
        spdlog::debug("{}", FUNC_SIG);
    }
};

} // namespace aloh::media::node