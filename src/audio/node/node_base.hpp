#pragma once

#include <audio/node/node.hpp>
#include <spdlog/spdlog.h>
#include <source_location>

namespace alo::audio::node{

template < typename T >
class NodeBase {

    protected:
    NodeBase() = default;

    public :

    Node* _upstream = nullptr;
    Node* _downstream = nullptr;
    bool _hot_path_log_b {false};

    void connect(Node* upstream, Node* downstream) {
        spdlog::debug("{}", std::source_location::current().function_name());
        
        if (upstream != nullptr){
            _upstream = upstream;
            
        }  else { spdlog::debug("{} | if (upstream != nullptr)", std::source_location::current().function_name()); }


        if (downstream != nullptr){
            _downstream = downstream;
            
        }  else { spdlog::debug("{} | if (downstream != nullptr)", std::source_location::current().function_name()); }

    }

    void start() {
        spdlog::debug("{}", std::source_location::current().function_name());

    }

    void build(){
        spdlog::debug("{}", std::source_location::current().function_name());

        if( !_downstream || !_upstream ){
            spdlog::error(" {} | if( !_downstream || !_upstream ) ", FUNC_SIG);
            return;
        }

        if( _downstream == _upstream ){
            spdlog::error(" {} | if( _downstream == _upstream ) WTF ??? ", FUNC_SIG);
            return;
        }

    }

    uint32_t pull(float* data, uint32_t frame_count){
        spdlog::debug("{}", std::source_location::current().function_name());

        if (_upstream == nullptr) {
            if (!_hot_path_log_b) {
                spdlog::error("{} | if(_upstream == nullptr)", FUNC_SIG);
                _hot_path_log_b = true;
            }
            return 0;
        }

        return _upstream->pull(data, frame_count);
    }

    void stop(){
        
    }

    uint32_t channels() const { 
        spdlog::debug("{}", std::source_location::current().function_name());

        if(_upstream == nullptr) { 
            spdlog::error("{} | if(_upstream == nullptr)", std::source_location::current().function_name());
            return 0;
        }

        return _upstream->channels();
    }

    uint32_t sample_rate() const {
        spdlog::debug("{}", std::source_location::current().function_name());
        
        if(_upstream == nullptr) { 
            spdlog::error("{} | if(_upstream == nullptr) ", std::source_location::current().function_name());
            return 0;
        }

        return _upstream->sample_rate();    
    }
};

} //namespace alo::audio::node