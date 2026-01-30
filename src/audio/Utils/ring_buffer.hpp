#pragma once

#include <cstdint>
#include <optional>
#include "AudioTypes.hpp"

namespace alo::audio::utils {

enum class RingBufferType { BYTE, PCM };

template <RingBufferType Mode>
class RingBuffer;

template <>
class RingBuffer<RingBufferType::PCM> {
public:

    static std::optional<RingBuffer<RingBufferType::PCM>> make(uint32_t channels, uint32_t size_in_frames);

    ~RingBuffer();

    RingBuffer(const RingBuffer&) = delete;
    RingBuffer& operator=(const RingBuffer&) = delete;
    
    RingBuffer(RingBuffer&& other) noexcept;
    RingBuffer& operator=(RingBuffer&& other) noexcept;
    
private:
    RingBuffer();
    bool _init(uint32_t channels, uint32_t size_in_frames);    
    pcm_rb_t _pcm_rb{};
};

} //audio::utils