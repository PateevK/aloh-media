#pragma once

#include "spdlog/spdlog.h"
#include <audio/Utils/AudioTypes.hpp>
#include <audio/Utils/ring_buffer.hpp>
#include <audio/node/node.hpp>

#include <cstdint>
#include <memory>
#include <optional>
#include <string>

// forward
struct ma_device;

namespace alo::audio{

using device_id_t = std::string;
using err_t = int;
enum class DeviceType { SRC, SINK };

template<DeviceType Type>    
class Device {
public:

    static std::shared_ptr<Device<Type>> make(device_info_ptr info,  device_id_t id, context_ref context);

    ~Device();
    
    device_id_t id() const { return _id; }
    void id(device_id_t id);
    bool is_init() const;
    bool is_started() const;
    
    uint32_t channels() const{
        return _num_channels;
    }

    uint32_t sample_rate() const{
        return _sample_rate;
    }

    void subscribe(Node* node){

        if(node == nullptr){
            spdlog::error("{} | if(node == nullptr)", FUNC_SIG);
            return;
        }

        _subscriber_nodes.push_back(node);
    }
    
    std::optional<err_t> init(); 
    void unInit();
    std::optional<err_t> start();
    std::optional<err_t> stop();

private:

    // Lets hope that sizeof(float) = 4 bytes
    // used and prealocated only in SINK devices
    std::vector<float> _mixer_preallocated_buff;

    Device(device_info_ptr info, device_id_t id, context_ref context);
    Device(Device&&) = default;
    Device& operator=(Device&&) = default;
    
    // Prevent copying (unique_ptr members make this non-copyable anyway)
    Device(const Device&) = delete;
    Device& operator=(const Device&) = delete;

    
    std::vector<Node*> _subscriber_nodes;
    bool _is_init = false;
    bool _is_started = false;
    device_id_t _id{};
    device_ptr _device = nullptr;
    device_info_ptr _info{};
    context_ref _context{};
    uint32_t _num_channels;
    uint32_t _sample_rate;

    static void _data_callback_c(ma_device* pDevice, void* pOutput, const void* pInput, uint32_t frameCount);
};

} // alo::audio