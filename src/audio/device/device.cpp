#include "device.hpp"
#include "audio/device/device.hpp"
#include "audio/Utils/AudioTypes.hpp"
#include "audio/Utils/AudioTypesImpl.hpp"

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
std::optional<err_t> Device<type>::init(const context_ptr& context){

    auto conf = ma_device_config_init(
        type == DeviceType::SRC ? ma_device_type_capture : ma_device_type_playback
    );

    if( !_info ){
        spdlog::error("{} | _info is NULL", FUNC_SIG);
        return 1;
    }

    conf.playback.pDeviceID = NULL;
    conf.playback.format    = ma_format_f32;
    conf.playback.channels  = 2;
        
    conf.dataCallback = _data_callback_c;
    conf.pUserData    = this;
    
    if(!context){
        spdlog::error("{} | context is false", FUNC_SIG);
        return 2;
    }

    auto result = ma_device_init(context->get(), &conf, _device->get());
    if(result != MA_SUCCESS){
        spdlog::error("{} | result != MA_SUCCESS", FUNC_SIG);
        return result;
    }

    return std::nullopt;
}

template<DeviceType type>
 std::optional<err_t> Device<type>::start(){
     auto result = ma_device_start(_device->get());
     
     if(result != MA_SUCCESS){
         _device.reset();
         return result;
     }
     return std::nullopt;
 }
 
 template<DeviceType type>
 std::optional<err_t> Device<type>::stop(){
     auto result = ma_device_stop(_device->get());
     if(result != MA_SUCCESS){
         _device.reset();
         return result;
     }
     return std::nullopt;
 }

template device_id_t Device<DeviceType::SINK>::id() const;
template device_id_t Device<DeviceType::SRC>::id() const;

} // alo::audio{