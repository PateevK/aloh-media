#include "ring_buffer.hpp"
#include "AudioTypes.hpp"

#include <cstring>
#include <miniaudio.h>
#include <audio/Utils/AudioTypesImpl.hpp>
#include <optional>

namespace alo::audio::utils {


    RingBuffer<RingBufferType::PCM>::RingBuffer() : _pcm_rb(nullptr), _channels(0) {
    }

    RingBuffer<RingBufferType::PCM>::RingBuffer(RingBuffer&& other) noexcept
        : _pcm_rb(std::move(other._pcm_rb)), _channels(other._channels) {
        other._channels = 0;
    }

    RingBuffer<RingBufferType::PCM>& RingBuffer<RingBufferType::PCM>::operator=(RingBuffer&& other) noexcept {
        if (this != &other) {
            _pcm_rb = std::move(other._pcm_rb);
            _channels = other._channels;
            other._channels = 0;
        }
        return *this;
    }

    bool RingBuffer<RingBufferType::PCM>::_init(uint32_t channels, uint32_t size_in_frames){
        auto res = pcm_rb::make(channels, size_in_frames);
        if(!res){
            return false;
        }

        _pcm_rb = std::move(res);
        _channels = channels;

        return true;
    }

    uint32_t RingBuffer<RingBufferType::PCM>::write(const float* data, uint32_t frame_count) {
        if (!_pcm_rb || !data || frame_count == 0) {
            return 0;
        }

        void* write_ptr = nullptr;
        ma_uint32 frames_to_write = frame_count;

        ma_result result = ma_pcm_rb_acquire_write(_pcm_rb->get(), &frames_to_write, &write_ptr);
        if (result != MA_SUCCESS || frames_to_write == 0) {
            return 0;
        }

        // Copy data to ring buffer (float samples = frames * channels)
        std::memcpy(write_ptr, data, frames_to_write * _channels * sizeof(float));

        result = ma_pcm_rb_commit_write(_pcm_rb->get(), frames_to_write);
        if (result != MA_SUCCESS) {
            return 0;
        }

        return frames_to_write;
    }

    uint32_t RingBuffer<RingBufferType::PCM>::read(float* data, uint32_t frame_count) {
        if (!_pcm_rb || !data || frame_count == 0) {
            return 0;
        }

        void* read_ptr = nullptr;
        ma_uint32 frames_to_read = frame_count;

        ma_result result = ma_pcm_rb_acquire_read(_pcm_rb->get(), &frames_to_read, &read_ptr);
        if (result != MA_SUCCESS || frames_to_read == 0) {
            return 0;
        }

        // Copy data from ring buffer (float samples = frames * channels)
        std::memcpy(data, read_ptr, frames_to_read * _channels * sizeof(float));

        result = ma_pcm_rb_commit_read(_pcm_rb->get(), frames_to_read);
        if (result != MA_SUCCESS) {
            return 0;
        }

        return frames_to_read;
    }

    uint32_t RingBuffer<RingBufferType::PCM>::available_read() const {
        if (!_pcm_rb) {
            return 0;
        }
        return ma_pcm_rb_available_read(_pcm_rb->get());
    }

    uint32_t RingBuffer<RingBufferType::PCM>::available_write() const {
        if (!_pcm_rb) {
            return 0;
        }
        return ma_pcm_rb_available_write(_pcm_rb->get());
    }

    RingBuffer<RingBufferType::PCM>::~RingBuffer() {
    }

    std::optional<RingBuffer<RingBufferType::PCM>> RingBuffer<RingBufferType::PCM>::make(uint32_t channels, uint32_t size_in_frames) {

        RingBuffer<RingBufferType::PCM> buffer;
        auto res = buffer._init(channels, size_in_frames);

        if(!res){
            return std::nullopt;
        }
        
        return buffer;
    }
    
}
