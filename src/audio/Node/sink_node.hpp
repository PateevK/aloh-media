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
        spdlog::debug("Sink::connect (terminal node)");
        if(next == nullptr){
            spdlog::warn("{} | if(next == nullptr)", FUNC_SIG);
        }
        _prev_node = next;
    }

    void push() const {
        std::println("Sink::push ???");
    }
    
    void pull() {
        spdlog::debug("{}", FUNC_SIG);
        _prev_node->pull();
    }

    static auto effect(Device<DeviceType::SINK>* device, void* output, const void* input, uint32_t frame_cout)-> void {
        spdlog::info("pupupupuuuuppupu");

    }

    void build() const {
        spdlog::debug("{}", FUNC_SIG);
        auto err = _device->init();
        if(err){
            spdlog::error("{} | err = {}", FUNC_SIG, err.value());
            return;
        }

        _device->cb(effect);

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