#pragma once

#include "spdlog/spdlog.h"
#include <cstdint>
#include <vector>

#include <audio/device/device.hpp>
#include <audio/engine/device_m.hpp>
#include <audio/node/node.hpp>
#include <audio/node/node_base.hpp>


namespace alo::audio::node{

namespace aa = alo::audio;

class Sink : public NodeBase<Sink> {
    aa::device_handle_t<aa::DeviceType::SINK> _device{};
    // Temp buffer for pulling from upstream before writing to device
    std::vector<float> _pull_buffer;

public:
    Sink(aa::device_handle_t<aa::DeviceType::SINK> device) : _device(device) {}

    uint32_t channels() const { return _device ? _device->channels() : 0; }
    uint32_t sample_rate() const { return _device ? _device->sample_rate() : 0; }

    void build(aa::Node* self){
        spdlog::debug("{}", FUNC_SIG);
        auto err = _device->init();
        if(err){
            spdlog::error("{} | err = {}", FUNC_SIG, err.value());
            return;
        }

        _device->subscribe(self);

        spdlog::debug("{} | {} | rate({}), ch({})", FUNC_SIG, _device->id(), _device->sample_rate(), _device->channels());
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