#include "device.hpp"
#include "audio/device/device.hpp"

namespace alo::audio{

template<DeviceType type>
device_id_t Device<type>::id() const {
    return _id;
}

template<DeviceType type>
void Device<type>::id(device_id_t id){
    _id = std::move(id);
}

template device_id_t Device<DeviceType::SINK>::id() const;
template device_id_t Device<DeviceType::SRC>::id() const;

} // alo::audio{