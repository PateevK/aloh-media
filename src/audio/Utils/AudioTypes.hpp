#pragma once
#include <memory>
#include <functional>

struct ma_device_wrapper;
struct ma_pcm_rb_wrapper;
struct ma_context_wrapper;
struct ma_device_info_wrapper;

struct DeviceDeleter {
    void operator()(ma_device_wrapper* device) const noexcept; 
};

struct MaPcmRbDeleter {
    void operator()(ma_pcm_rb_wrapper* rb) const noexcept; 
};

struct ContextDeleter {
    void operator()(ma_context_wrapper* device) const noexcept; 
};

struct DeviceInfoDeleter {
    void operator()(ma_device_info_wrapper* info) const noexcept;
};

using device_ptr = std::unique_ptr<ma_device_wrapper, DeviceDeleter>;
using device_t = device_ptr;

using device_info_ptr = std::unique_ptr<ma_device_info_wrapper, DeviceInfoDeleter>;

namespace device {
    auto make() noexcept -> device_ptr;
}

using device_data_callback = std::function<void(ma_device_wrapper* pDevice, void* pOutput, const void* pInput, uint32_t frameCount)>;

namespace device::info {
    auto make() noexcept -> device_info_ptr;
}

using pcm_rb_ptr = std::unique_ptr<ma_pcm_rb_wrapper, MaPcmRbDeleter>;
using pcm_rb_t = pcm_rb_ptr;


namespace pcm_rb{
    auto make(uint32_t channels, uint32_t size_in_frames) noexcept -> pcm_rb_ptr;
}
using context_ptr = std::unique_ptr<ma_context_wrapper, ContextDeleter>;
using context_t = context_ptr;
using context_ref = const context_t&;

namespace context{
    auto make() noexcept -> context_t ;
}