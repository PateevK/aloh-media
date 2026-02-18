#pragma once 

#include <audio/device/device.hpp>
#include <audio/Utils/AudioTypes.hpp>

#include <vector>
#include <unordered_map>

namespace alo::audio {

template<DeviceType type>
using device_handle_t = std::shared_ptr<Device<type>>;

template<DeviceType type>
using device_container_t = std::unordered_map<device_id_t, device_handle_t<type>>;

class DeviceM{
    public:
    
    void init();
    void uninit();
    
    template<DeviceType type>
    std::vector<device_id_t> ids() const;

    template<DeviceType type>
    const device_handle_t<type> get(const device_id_t& id) const;

    private:

    void _updateDevices();

    context_t _context{};
    device_container_t<DeviceType::SINK> _sink_container;
    device_container_t<DeviceType::SRC> _src_container;

};

} // namespace alo::audio
