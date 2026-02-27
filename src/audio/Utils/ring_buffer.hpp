#pragma once

#include <cstdint>
#include <optional>
#include "AudioTypes.hpp"

namespace alo::audio::utils {

enum class RingBufferType { BYTE, PCM };

template <RingBufferType Mode>
class RingBuffer;


template <>
class RingBuffer<RingBufferType::BYTE> {
public:

    static std::optional<RingBuffer<RingBufferType::BYTE>> make(uint32_t sz_in_bytes);

    ~RingBuffer();

    RingBuffer(const RingBuffer&) = delete;
    RingBuffer& operator=(const RingBuffer&) = delete;
    
    RingBuffer(RingBuffer&& other) noexcept;
    RingBuffer& operator=(RingBuffer&& other) noexcept;

    uint32_t write(const float* data, uint32_t frame_count);

    uint32_t read(float* data, uint32_t frame_count);

    uint32_t available_read() const;

    uint32_t available_write() const;
    
    RingBuffer();
private:
    bool _init(uint32_t size_in_bytes);    
    rb_t _rb{};
};

template <>
class RingBuffer<RingBufferType::PCM> {
public:

    static std::optional<RingBuffer<RingBufferType::PCM>> make(uint32_t channels, uint32_t size_in_frames);
    

    ~RingBuffer();

    RingBuffer(const RingBuffer&) = delete;
    RingBuffer& operator=(const RingBuffer&) = delete;
    
    RingBuffer(RingBuffer&& other) noexcept;
    RingBuffer& operator=(RingBuffer&& other) noexcept;

    // Write frames to the ring buffer
    // Returns the number of frames actually written
    uint32_t write(const float* data, uint32_t frame_count);

    // Read frames from the ring buffer
    // Returns the number of frames actually read
    uint32_t read(float* data, uint32_t frame_count);

    // Returns number of frames available for reading
    uint32_t available_read() const;

    // Returns number of frames available for writing
    uint32_t available_write() const;
    
    RingBuffer();
private:
    bool _init(uint32_t channels, uint32_t size_in_frames);    
    pcm_rb_t _pcm_rb{};
    uint32_t _channels{0};
};

} //audio::utils