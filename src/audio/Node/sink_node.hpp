#pragma once

#include <print>

#include <audio/device/device.hpp>
#include <audio/engine/device_m.hpp>
#include <audio/node/node.hpp>


namespace alo::audio::node{

namespace aa = alo::audio;

class Sink{
    aa::device_handle_t<aa::DeviceType::SINK> _device{};
    // Sink is terminal - no next node needed

public:
    Sink(aa::device_handle_t<aa::DeviceType::SINK> device) : _device(device) {}

    void connect(Node* next) {
        // Sink is a terminal node, typically doesn't connect to anything
        std::println("Sink::connect (terminal node)");
    }

    void push() const {
        std::println("Sink::push");
    }
    
    void pull() const {
        std::println("Sink::pull");
    }

    void build() const {
        std::println("Sink::build");
    }
    
    void start() const {
        std::println("Sink::start");
    }
};

} // namespace aloh::media::node