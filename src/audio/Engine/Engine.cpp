#include "Engine.hpp"
#include "audio/device/device.hpp"
#include "device_m.hpp"

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

        auto Engine::device_m() const -> const DeviceM& {
                return _device_m;
        }

} // audio

} // alo