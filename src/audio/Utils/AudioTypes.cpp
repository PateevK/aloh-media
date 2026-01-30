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