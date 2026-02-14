#pragma once 

#include <spdlog/spdlog.h>

#include "node.hpp"

namespace alo::audio::node{

class Converter{
    Node* _upstream = nullptr;
    Node* _downstream = nullptr;
    bool _hot_path_log_b{false};

public:
    void connect(Node* upstream, Node* downstream) {
        spdlog::debug("Converter::connect");
        if (upstream != nullptr)  _upstream = upstream;
        if (downstream != nullptr) _downstream = downstream;
    }

    void start() {
    }

    void build(){
    }

    uint32_t pull(float* data, uint32_t frame_count){
        if (_upstream == nullptr) {
            if (!_hot_path_log_b) {
                spdlog::error("{} | if(_upstream == nullptr)", FUNC_SIG);
                _hot_path_log_b = true;
            }
            return 0;
        }
        return _upstream->pull(data, frame_count);
    }

};


} // alo::audio::node

