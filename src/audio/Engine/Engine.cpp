#include "Engine.hpp"

#include <miniaudio.h>
#include <spdlog/spdlog.h>

namespace alo{

namespace audio{

        void Engine::init(){
            _device_m.init();
        }

        void Engine::start(){
        }

        void Engine::stop(){
                
        }

        void Engine::uninit(){
                _device_m.uninit();
        }

        const auto& Engine::device_m() const{
                return _device_m;
        }

} // audio

} // alo