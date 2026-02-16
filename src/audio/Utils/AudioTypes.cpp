#include "AudioTypes.hpp"
#include "AudioTypesImpl.hpp"
#include "audio/Utils/AudioTypesImpl.hpp"

#include <miniaudio.h>
#include <spdlog/spdlog.h>

#include <memory>

void DeviceDeleter::operator()(ma_device_wrapper* device) const noexcept {
    if (device != nullptr) {
        ma_device_uninit(device->get());
        delete device;
    }
}

void MaPcmRbDeleter::operator()(ma_pcm_rb_wrapper* rb) const noexcept {
    if (rb != nullptr) {
        ma_pcm_rb_uninit(rb->get());
        delete rb;
    }
};

void ContextDeleter::operator()(ma_context_wrapper* con) const noexcept{
     if (con != nullptr) {
        ma_context_uninit(con->get());
        delete con;
    }
}

void DeviceInfoDeleter::operator()(ma_device_info_wrapper* info) const noexcept {
    delete info;  // no uninit needed, it's just plain data
} 

void ChConverterDeleter::operator()(ma_channel_converter_wrapper* converter) const noexcept {
    if(converter != nullptr){
        ma_channel_converter_uninit(converter->get(), NULL);
        delete converter;
    }
}

void ResamplerDeleter::operator()(ma_resampler_wrapper* resampler) const noexcept {
    if (resampler != nullptr) {
        ma_linear_resampler_uninit(resampler->get(), NULL);
        delete resampler;
    }
}

auto device::make() noexcept -> device_ptr {
    return device_ptr{ new (std::nothrow) ma_device_wrapper{} };
}

auto device::info::make() noexcept -> device_info_ptr {
    return device_info_ptr{ new (std::nothrow) ma_device_info_wrapper{} };
}


auto pcm_rb::make(uint32_t channels, uint32_t size_in_frames) noexcept -> pcm_rb_ptr {
    pcm_rb_ptr ptr{ new (std::nothrow) ma_pcm_rb_wrapper{} };


    if(!ptr){
        spdlog::critical("pcm_rb::make() : alloc failed");
        return pcm_rb_ptr{};
    }

    auto res = ma_pcm_rb_init(ma_format_f32, channels, size_in_frames, NULL, NULL, ptr->get());

    if(res != MA_SUCCESS){
        spdlog::critical("pcm_rb::make() : ma_pcm_rb_init failed");
        delete ptr.release();
        return pcm_rb_ptr{};
    }

    return ptr;
}


auto context::make() noexcept -> context_t {
    context_ptr ptr{ new (std::nothrow) ma_context_wrapper{} };

    if(!ptr){
        spdlog::critical("Context::make() : alloc failed");
        return context_ptr{};
    }

    auto res = ma_context_init(NULL, 0, NULL, ptr->get());

    if(res != MA_SUCCESS){
        spdlog::critical("Context::make() : ma_context_init failed");
        delete ptr.release();
        return context_ptr{};
    }

    return ptr;
}

auto ch_converter::make(uint32_t ch_in, uint32_t ch_out) noexcept -> ch_converter_ptr {
    ch_converter_ptr ptr{ new (std::nothrow) ma_channel_converter_wrapper{} };

    ma_channel_converter_config cfg = ma_channel_converter_config_init(ma_format_f32, ch_in, NULL, ch_out, NULL, ma_channel_mix_mode_default);
    ma_channel_converter converter;
    auto res = ma_channel_converter_init(&cfg, NULL, ptr->get());
    
    if(res != MA_SUCCESS){
        return ch_converter_ptr{};
    }
    
    return ptr;
}

auto resampler::make(uint32_t channels, uint32_t sample_rate_in, uint32_t sample_rate_out) noexcept -> resampler_ptr {
    if (sample_rate_in == 0 || sample_rate_out == 0) {
        return resampler_ptr{};
    }
    resampler_ptr ptr{ new (std::nothrow) ma_resampler_wrapper{} };
    if (!ptr) {
        return resampler_ptr{};
    }
    ma_linear_resampler_config cfg = ma_linear_resampler_config_init(ma_format_f32, channels, sample_rate_in, sample_rate_out);
    ma_result res = ma_linear_resampler_init(&cfg, NULL, ptr->get());
    if (res != MA_SUCCESS) {
        return resampler_ptr{};
    }
    return ptr;
}

void resample(resampler_ptr const& resampler, const void* frames_in, uint64_t* frame_count_in, void* frames_out, uint64_t* frame_count_out) {
    if (!resampler || !frames_in || !frame_count_in || !frames_out || !frame_count_out) {
        return;
    }
    ma_linear_resampler_process_pcm_frames(resampler->get(), frames_in, frame_count_in, frames_out, frame_count_out);
}

void convertchannels(ch_converter_ptr const& converter, void* frames_out, const void* frames_in, uint64_t frame_count) {
    if (!converter || !frames_out || !frames_in) {
        return;
    }
    ma_channel_converter_process_pcm_frames(converter->get(), frames_out, frames_in, frame_count);
}
