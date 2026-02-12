#pragma once

#include <print>

#include <audio/device/device.hpp>
#include <audio/engine/device_m.hpp>
#include <audio/node/node.hpp>

namespace alo::audio::node{

namespace aa = alo::audio;

class Src{
    aa::device_handle_t<aa::DeviceType::SRC> _device{};
    Node* _next = nullptr;

public:
    Src(aa::device_handle_t<aa::DeviceType::SRC> device) : _device(device) {}
        
    void connect(Node* next) {
        _next = next;
        std::println("Src::connect -> next node");
    }

    void push() const {
        std::println("Src::push");
    }
    
    void pull() const {
        std::println("Src::pull");
    }
    
    void build() const {
        std::println("Src::build");
    }
    
    void start() {
        std::println("Src::start");
    }
};
 
} // namespace aloh::media::node