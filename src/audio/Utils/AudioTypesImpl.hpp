#pragma once

#include <miniaudio.h>
#include "AudioTypes.hpp"

struct ma_device_wrapper{
    ma_device device;
    ma_device* get() { return &device; }
};

struct ma_device_info_wrapper{
    ma_device_info _info;
    ma_device_info* get() { return &_info; }
};

struct ma_pcm_rb_wrapper{
    ma_pcm_rb pcm_rb;
    ma_pcm_rb* get() { return &pcm_rb; }
};

struct ma_context_wrapper{
    ma_context context;
    ma_context* get() { return &context; }
};

struct ma_channel_converter_wrapper{
    ma_channel_converter converter;
    ma_channel_converter* get() { return &converter; }
};

struct ma_resampler_wrapper{
    ma_linear_resampler resampler;
    ma_linear_resampler* get() { return &resampler; }
};