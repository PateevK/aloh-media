#pragma once
#include <cstdint>
#include <memory>
#include <functional>

struct ma_device_wrapper;
struct ma_pcm_rb_wrapper;
struct ma_context_wrapper;
struct ma_device_info_wrapper;
struct ma_channel_converter_wrapper;
struct ma_resampler_wrapper;

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

struct ChConverterDeleter {
    void operator()(ma_channel_converter_wrapper* info) const noexcept;
};

struct ResamplerDeleter {
    void operator()(ma_resampler_wrapper* resampler) const noexcept;
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

using ch_converter_t = ma_channel_converter_wrapper;
using ch_converter_ptr = std::unique_ptr<ma_channel_converter_wrapper, ChConverterDeleter>;
namespace ch_converter{
    auto make(uint32_t ch_in, uint32_t ch_out) noexcept -> ch_converter_ptr;
}

using resampler_ptr = std::unique_ptr<ma_resampler_wrapper, ResamplerDeleter>;
namespace resampler{
    auto make(uint32_t channels, uint32_t sample_rate_in, uint32_t sample_rate_out) noexcept -> resampler_ptr;
}

void resample(resampler_ptr const& resampler, const void* frames_in, uint64_t* frame_count_in, void* frames_out, uint64_t* frame_count_out);
void convertchannels(ch_converter_ptr const& converter, void* frames_out, const void* frames_in, uint64_t frame_count);
