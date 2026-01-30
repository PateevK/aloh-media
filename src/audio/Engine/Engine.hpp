#pragma once

#include <audio/Pipeline/Pipeline.hpp>
#include "device_m.hpp"



namespace alo::audio{

    class Engine{
        public:

        Engine()  = default;
        ~Engine() = default;

        void init(); 
        void start();
        void stop();
        void uninit(); 
        const auto& device_m() const; 
    
        private:

        DeviceM _device_m{};

    };

} // alo::audio