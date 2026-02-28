#pragma once 

#include <cstdint>
#include <cstring>
#include <spdlog/spdlog.h>

#include "node_base.hpp"
#include <audio/Utils/ring_buffer.hpp>

namespace alo::audio::node{

class Buffer : public NodeBase<Buffer> {
    uint32_t _channels{0};
    uint32_t _sampla_rate{0};
    alo::audio::utils::RingBuffer<alo::audio::utils::RingBufferType::PCM> _buffer;

public:

    Buffer() = default;

    uint32_t pull(float* data, uint32_t frame_count){

        if (_downstream == nullptr) {
            if (!_hot_path_log_b) {
                spdlog::error("{} | if(_downstream == nullptr)", FUNC_SIG);
                _hot_path_log_b = true;
            }
            
            return 0; 
        }
    
        uint32_t available_frames = _buffer.available_read();
        
        // Determine exactly how much we can safely read
        uint32_t frames_to_read = std::min(available_frames, frame_count);
        
        if (frames_to_read > 0) {
            _buffer.read(data, frames_to_read);
        }
    
        // Handle buffer underrun (we need more frames than we have)
        if (frames_to_read < frame_count) {
            uint32_t missing_frames = frame_count - frames_to_read;
            uint32_t missing_samples = missing_frames * _channels;
            
            // Offset the pointer to start writing silence AFTER the valid audio we just read
            float* padding_start = data + (frames_to_read * _channels);
            
            // memset expects bytes, not frames or samples
            std::memset(padding_start, 0, missing_samples * sizeof(float));
        }
    
        // Return the actual number of valid frames provided (capped at frame_count)
        return frames_to_read;
    }

    void push(float* data, uint32_t frame_count){

        if (_upstream == nullptr) {
            if (!_hot_path_log_b) {
                // Fixed the copy-pasted log message
                spdlog::error("{} | if(_upstream == nullptr)", FUNC_SIG); 
                _hot_path_log_b = true;
            }
            return;
        }
    
        // Determine how much space is actually available in the buffer
        uint32_t available_space = _buffer.available_write();
        
        // Calculate exactly how many frames we can safely write
        uint32_t frames_to_write = std::min(available_space, frame_count);
    
        if (frames_to_write > 0) {
            // Write the data into our internal buffer, NOT directly to downstream
            _buffer.write(data, frames_to_write);
        }
    
        // Handle buffer overrun (we were handed more frames than we have space for)
        if (frames_to_write < frame_count) {
            if (!_hot_path_log_b) {
                uint32_t dropped_frames = frame_count - frames_to_write;
                spdlog::warn("{} | Buffer overrun! Dropping {} frames.", FUNC_SIG, dropped_frames);
                _hot_path_log_b = true; // Latch the log to prevent console spam on the audio thread
            }
        }
    }

    
    void build(Node* /*self*/){
        spdlog::debug("{}", std::source_location::current().function_name());

        if( !_downstream || !_upstream ){
            spdlog::error(" {} | if( !_downstream || !_upstream ) ", FUNC_SIG);
            return;
        }

        if( _downstream == _upstream ){
            spdlog::error(" {} | if( _downstream == _upstream ) WTF ??? ", FUNC_SIG);
            return;
        }

        _channels = _downstream->channels();
        _sampla_rate = _downstream->sample_rate();

        auto res = alo::audio::utils::RingBuffer<alo::audio::utils::RingBufferType::PCM>::make(_channels, _sampla_rate);

        if(!res){
            spdlog::error("{} | if(!res)", FUNC_SIG);
            return;
        }

        _buffer = std::move(res.value());

        spdlog::info(
            "{} | _channels({}), _sampla_rate({})", 
            FUNC_SIG, _channels, _sampla_rate
        );
    }


};


} // alo::audio::node

