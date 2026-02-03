#include "engine.hpp"
#include "device_m.hpp"

#include <miniaudio.h>
#include <spdlog/spdlog.h>

namespace alo{

namespace audio{

        void Engine::init(){
            device.init();
        }

        void Engine::start(){

        }

        void Engine::stop(){
                
        }

        void Engine::uninit(){
                device.uninit();
        }

} // audio

} // alo