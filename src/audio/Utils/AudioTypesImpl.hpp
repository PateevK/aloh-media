#pragma once

#include "AudioTypes.hpp"
#include <miniaudio.h>

struct ma_device_wrapper{
    ma_device device;
    ma_device* get() { return &device; }
};

struct ma_pcm_rb_wrapper{
    ma_pcm_rb pcm_rb;
    ma_pcm_rb* get() { return &pcm_rb; }
};

struct ma_context_wrapper{
    ma_context context;
    ma_context* get() { return &context; }
};