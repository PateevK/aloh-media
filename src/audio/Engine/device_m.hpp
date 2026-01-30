#pragma once 

#include <audio/Device/device.hpp>
#include <audio/Utils/AudioTypes.hpp>

#include <vector>

namespace alo::audio {

class DeviceM{
    public:
    
    void init();
    void uninit();
    
    std::vector<device_id_t> device() const;
    
    template<DeviceType type>
    Device<type> device() const;

    private:

    void _updateDevices();

    context_t _context{};
    std::vector<Device<DeviceType::SINK>> _sink;
    std::vector<Device<DeviceType::SRC>> _src;

};

} // namespace alo::audio