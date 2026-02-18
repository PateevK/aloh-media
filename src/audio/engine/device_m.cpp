#include "device_m.hpp"
#include "audio/Utils/AudioTypes.hpp"
#include "audio/device/device.hpp"

#include <algorithm>
#include <audio/Utils/AudioTypesImpl.hpp>

#include <iterator>
#include <miniaudio.h>
#include <spdlog/spdlog.h>

#include <type_traits>
#include <tuple>

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
            return i.second->id();
        };

        if constexpr (type == DeviceType::SINK){
            std::ranges::transform(_sink_container, std::back_inserter(ids), get_id);
            return std::move(ids);
        }
        else if constexpr (type == DeviceType::SRC){
            std::ranges::transform(_src_container, std::back_inserter(ids), get_id);
            return std::move(ids);
        }
        else {
            static_assert(always_false<type>::value, "Unhandled DeviceType in ids()");
        }
    }

    // Be carefull )
    template<DeviceType type>
    const device_handle_t<type> DeviceM::get(const device_id_t& id) const{

        if constexpr (type == DeviceType::SINK){
            auto it = _sink_container.find(id);
            if (it != _sink_container.end()) {
                return it->second;
            }
        }
        else if constexpr (type == DeviceType::SRC){
            auto it = _src_container.find(id);
            if (it != _src_container.end()) {
                return it->second;
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

        auto deviceInfo = [&]( int index, ma_device_type device_type, ma_device_info* infos){
            auto info = device::info::make();
            auto result = ma_context_get_device_info(
                _context->get(), 
                device_type, 
                &infos[index].id, 
                info->get()
            );
            
            if (result != MA_SUCCESS) {
                spdlog::warn("{} | Failed to get device info for playback device {}", FUNC_SIG, index);
                *info->get() = infos[index]; // Fallback to basic info
            }

            std::string device_id(info->get()->name);

            return std::make_tuple(device_id, std::move(info));
        };

        for (ma_uint32 iDevice = 0; iDevice < playbackCount; iDevice += 1) {
            auto [device_id, info] = deviceInfo(iDevice, ma_device_type_playback, pPlaybackInfos);
            
            _sink_container.try_emplace(
                device_id, 
                Device<DeviceType::SINK>::make(std::move(info), device_id, _context)
            );
        }

        for (ma_uint32 iDevice = 0; iDevice < captureCount; iDevice += 1) {
            auto [device_id, info] = deviceInfo(iDevice, ma_device_type_capture, pCaptureInfos);

            _src_container.try_emplace(
                device_id, 
                Device<DeviceType::SRC>::make(std::move(info), device_id, _context)
            );
        }
    }

        // Explicit template instantiations
    template const device_handle_t<DeviceType::SINK> DeviceM::get<DeviceType::SINK>(const device_id_t&) const;
    template const device_handle_t<DeviceType::SRC> DeviceM::get<DeviceType::SRC>(const device_id_t&) const;

    template std::vector<device_id_t> DeviceM::ids<DeviceType::SINK>() const;
    template std::vector<device_id_t> DeviceM::ids<DeviceType::SRC>() const;


} // alo::audio