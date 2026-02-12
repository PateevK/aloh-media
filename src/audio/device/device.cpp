#include "device.hpp"
#include "audio/device/device.hpp"
#include "audio/Utils/AudioTypes.hpp"
#include "audio/Utils/AudioTypesImpl.hpp"

#include <optional>
#include <spdlog/spdlog.h>

namespace alo::audio{

template<DeviceType type>
device_id_t Device<type>::id() const {
    return _id;
}

template<DeviceType type>
void Device<type>::id(device_id_t id){
    _id = std::move(id);
}

template<DeviceType type>
bool Device<type>::is_init() const {
    return _is_init;
}

template<DeviceType type>
bool Device<type>::is_started() const {
    return _is_started;
}

template<DeviceType type>
std::optional<err_t> Device<type>::init(){
    if(is_init()){
        return std::nullopt;
    }

    auto conf = ma_device_config_init(
        type == DeviceType::SRC ? ma_device_type_capture : ma_device_type_playback
    );

    if( !_info ){
        spdlog::error("{} | _info is NULL", FUNC_SIG);
        return 1;
    }

    conf.playback.pDeviceID = &_info->get()->id;
    conf.playback.format    = ma_format_f32;
    conf.playback.channels  = 2;
        
    conf.dataCallback = _data_callback_c;
    conf.pUserData    = this;
    
    if(!_context){
        spdlog::error("{} | if(!_context)", FUNC_SIG);
        return 2;
    }

    auto result = ma_device_init(_context->get(), &conf, _device->get());
    if(result != MA_SUCCESS){
        spdlog::error("{} | result != MA_SUCCESS", FUNC_SIG);
        return result;
    }

    _is_init = true;
    return std::nullopt;
}

template<DeviceType type>
Device<type>::~Device(){
    auto res = stop();
    if(res){
        spdlog::error("{} | device is not stopped, forcing unInit()", FUNC_SIG);
    }
    unInit();
}

template<DeviceType type>
 std::optional<err_t> Device<type>::start(){
    
    if(!is_init()){
        spdlog::error(" {} | init device first", FUNC_SIG);
        return 1;
    } 

    auto result = ma_device_start(_device->get());
     
     if(result != MA_SUCCESS){
         _device.reset();
         return result;
     }

     _is_started = true;
     return std::nullopt;
 }
 
 template<DeviceType type>
 std::optional<err_t> Device<type>::stop(){
     auto result = ma_device_stop(_device->get());
     if(result != MA_SUCCESS){
         _device.reset();
         return result;
     }

     _is_started = false;
     return std::nullopt;
 }

template<DeviceType type>
void Device<type>::unInit() {
    if(!is_init()){
        return;
    }
    if(is_started()){
        stop();
    }
    ma_device_uninit(_device->get());
}


template bool Device<DeviceType::SRC>::is_started() const;
template bool Device<DeviceType::SINK>::is_started() const;
template bool Device<DeviceType::SRC>::is_init() const;
template bool Device<DeviceType::SINK>::is_init() const;
template std::optional<err_t> Device<DeviceType::SINK>::init();
template std::optional<err_t> Device<DeviceType::SRC>::init();
template void Device<DeviceType::SRC>::unInit();
template void Device<DeviceType::SINK>::unInit();
template std::optional<err_t>  Device<DeviceType::SINK>::start();
template std::optional<err_t>  Device<DeviceType::SINK>::stop();
template std::optional<err_t>  Device<DeviceType::SRC>::start();
template std::optional<err_t>  Device<DeviceType::SRC>::stop();
template device_id_t Device<DeviceType::SINK>::id() const;
template device_id_t Device<DeviceType::SRC>::id() const;
template Device<DeviceType::SINK>::~Device();
template Device<DeviceType::SRC>::~Device();

} // alo::audio{