#pragma once

#include <audio/Pipeline/Pipeline.hpp>
#include "device_m.hpp"
#include "pipeline_m.hpp"

namespace alo::audio{

    class Engine{
        public:

        Engine()  = default;
        ~Engine() = default;

        void init(); 
        void start();
        void stop();
        void uninit(); 
        
        PipelineM pipeline{this};
        DeviceM device{};
        private:
        
    };

} // alo::audio