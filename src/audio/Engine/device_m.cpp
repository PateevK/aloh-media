#include "device_m.hpp"

#include <audio/Utils/AudioTypesImpl.hpp>

#include <miniaudio.h>
#include <spdlog/spdlog.h>

#include <print>

namespace  alo::audio {

    void DeviceM::init(){
        _context = context::make();
        _updateDevices();
    }

    void DeviceM::uninit(){

    }

    void DeviceM::_updateDevices(){
        
        ma_device_info* pPlaybackInfos;
        ma_uint32 playbackCount;
        ma_device_info* pCaptureInfos;
        ma_uint32 captureCount;
        
        auto res = ma_context_get_devices(_context->get(), &pPlaybackInfos, &playbackCount, &pCaptureInfos, &captureCount); 
        
        if(res != MA_SUCCESS) {
            spdlog::error("{} - ma_context_get_devices res == {}", std::string(FUNC_SIG), static_cast<int>(res));
        }

        for (ma_uint32 iDevice = 0; iDevice < playbackCount; iDevice += 1) {
            auto info = device::info::make();
            *info->get() = pPlaybackInfos[iDevice];

            _sink.emplace_back(std::move(info));
        }

        for (ma_uint32 iDevice = 0; iDevice < captureCount; iDevice += 1) {
            auto info = device::info::make();
            *info->get() = pCaptureInfos[iDevice];

            _src.emplace_back(std::move(info));
        }

    }

} // alo::audio