#include "ring_buffer.hpp"
#include "AudioTypes.hpp"


#include <miniaudio.h>
#include <audio/Utils/AudioTypesImpl.hpp>
#include <optional>

namespace alo::audio::utils {


    RingBuffer<RingBufferType::PCM>::RingBuffer() : _pcm_rb(nullptr) {
    }

    RingBuffer<RingBufferType::PCM>::RingBuffer(RingBuffer&& other) noexcept
        : _pcm_rb(std::move(other._pcm_rb)) {
    }

    RingBuffer<RingBufferType::PCM>& RingBuffer<RingBufferType::PCM>::operator=(RingBuffer&& other) noexcept {
        if (this != &other) {
            _pcm_rb = std::move(other._pcm_rb);
        }
        return *this;
    }

    bool RingBuffer<RingBufferType::PCM>::_init(uint32_t channels, uint32_t size_in_frames){
        auto res = pcm_rb::make(channels, size_in_frames);
        if(!res){
            return false;
        }

        _pcm_rb = std::move(res);

        return true;
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
