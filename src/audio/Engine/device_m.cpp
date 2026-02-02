#include "device_m.hpp"
#include "audio/device/device.hpp"

#include <algorithm>
#include <audio/Utils/AudioTypesImpl.hpp>

#include <iterator>
#include <miniaudio.h>
#include <spdlog/spdlog.h>

#include <type_traits>
#include <unordered_map>

template<auto> 
struct always_false : std::false_type {};

namespace  alo::audio {

    void DeviceM::init(){
        _context = context::make();
        _updateDevices();
    }

    void DeviceM::uninit(){

    }
    
    template<DeviceType type>
    std::vector<device_id_t> DeviceM::ids() const{
        std::vector<device_id_t> ids;

        auto get_id = [](const auto& i){
            return i.second.id();
        };

        if constexpr (type == DeviceType::SINK){
            std::ranges::transform(_sink, std::back_inserter(ids), get_id);
            return std::move(ids);
        }
        else if constexpr (type == DeviceType::SRC){
            std::ranges::transform(_src, std::back_inserter(ids), get_id);
            return std::move(ids);
        }
        else {
            static_assert(always_false<type>::value, "Unhandled DeviceType in ids()");
        }
    }

    // Be carefull )
    template<DeviceType type>
    const device_handle_t<type>* DeviceM::device(const device_id_t& id) const{

        if constexpr (type == DeviceType::SINK){
            auto it = _sink.find(id);
            if (it != _sink.end()) {
                return &it->second;
            }
        }
        else if constexpr (type == DeviceType::SRC){
            auto it = _src.find(id);
            if (it != _src.end()) {
                return &it->second;
            }
        }
        else {
            static_assert(always_false<type>::value, "Unhandled DeviceType in device()");
        }

        return nullptr;
    }

    void DeviceM::_updateDevices(){
        
        ma_device_info* pPlaybackInfos;
        ma_uint32 playbackCount;
        ma_device_info* pCaptureInfos;
        ma_uint32 captureCount;
        
        auto res = ma_context_get_devices(_context->get(), &pPlaybackInfos, &playbackCount, &pCaptureInfos, &captureCount); 
        
        if(res != MA_SUCCESS) {
            spdlog::error("{} - ma_context_get_devices res == {}", std::string(FUNC_SIG), static_cast<int>(res));
            return;
        }

        for (ma_uint32 iDevice = 0; iDevice < playbackCount; iDevice += 1) {
            auto info = device::info::make();
            *info->get() = pPlaybackInfos[iDevice];

            std::string device_id(info->get()->name);
            _sink.try_emplace(device_id, std::move(info), device_id);
        }

        for (ma_uint32 iDevice = 0; iDevice < captureCount; iDevice += 1) {
            auto info = device::info::make();
            *info->get() = pCaptureInfos[iDevice];

            std::string device_id(info->get()->name);
            _src.try_emplace(device_id, std::move(info), device_id);
        }

        
    }

        // Explicit template instantiations
    template const device_handle_t<DeviceType::SINK>* DeviceM::device<DeviceType::SINK>(const device_id_t&) const;
    template const device_handle_t<DeviceType::SRC>* DeviceM::device<DeviceType::SRC>(const device_id_t&) const;

    template std::vector<device_id_t> DeviceM::ids<DeviceType::SINK>() const;
    template std::vector<device_id_t> DeviceM::ids<DeviceType::SRC>() const;


} // alo::audio