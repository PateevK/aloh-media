#pragma once 

#include <vector>
#include <spdlog/spdlog.h>

#include "node.hpp"
#include "node_base.hpp"
#include <audio/Utils/AudioTypes.hpp>
#include <audio/Utils/AudioTypesImpl.hpp>

namespace alo::audio::node{

class Converter : public NodeBase<Converter> {
    ch_converter_ptr _ch_converter;
    resampler_ptr _resampler;
    uint32_t _ch_in{0};
    uint32_t _ch_out{0};
    uint32_t _sr_in{0};
    uint32_t _sr_out{0};
    std::vector<float> _pull_buf;
    std::vector<float> _convert_buf;
    std::vector<float> _push_buf;

    auto _initChannels(uint32_t ch_in, uint32_t ch_out){
        _ch_converter = ch_converter::make(ch_in, ch_out);
        if(!_ch_converter){
            spdlog::error(" {} | if(!_ch_converter)", FUNC_SIG);
        }
    }

    auto _initSamples(uint32_t channels, uint32_t sr_in, uint32_t sr_out){
        _resampler = resampler::make(channels, sr_in, sr_out);
        if(!_resampler){
            spdlog::error(" {} | if(!_resampler)", FUNC_SIG);
        }
    }

    

public:

    Converter() = default;

    Converter(const Node& src, const Node& sink) : 
    _ch_in(src.channels()), _ch_out(sink.channels()), _sr_in(src.sample_rate()), _sr_out(sink.sample_rate()){};

    Converter(uint32_t ch_in,  uint32_t sr_in, uint32_t ch_out,   uint32_t sr_out)
        : _ch_in(ch_in), _ch_out(ch_out), _sr_in(sr_in), _sr_out(sr_out){};

    void build(Node* /*self*/){

        if( !_downstream || !_upstream ){
            spdlog::error(" {} | if( !_downstream || !_upstream ) ", FUNC_SIG);
            return;
        }

        if( _downstream == _upstream ){
            spdlog::error(" {} | if( _downstream == _upstream ) WTF ??? ", FUNC_SIG);
            return;
        }

        auto up_ch = _upstream->channels();
        auto up_smpl = _upstream->sample_rate();
        auto down_ch =  _downstream->channels();
        auto down_smpl = _downstream->sample_rate();

        if (up_ch == 0 || down_ch == 0) {
            spdlog::error(" {} | if (_sr_in == 0 && _sr_out == 0)", FUNC_SIG);
            return;
        }
          
        if (up_smpl == 0 || down_smpl == 0) {
            spdlog::error(" {} | if (_ch_in == 0 && _ch_out == 0) ", FUNC_SIG);
            return;
        }

        _ch_in  = up_ch;
        _ch_out = down_ch;
        _sr_in  = up_smpl;
        _sr_out = down_smpl;

        _initChannels(up_ch, down_ch);
        _initSamples(down_ch, up_smpl, down_smpl);

        spdlog::info(
            "{} | ch-in({}), sr_in({}), ch-out({}), sr-out({})", 
            FUNC_SIG, _ch_in, _sr_in, _ch_out, _sr_out
        );
    }

    uint32_t pull(float* data, uint32_t frame_count){
        if (_upstream == nullptr) {
            if (!_hot_path_log_b) {
                spdlog::error("{} | if(_upstream == nullptr)", FUNC_SIG);
                _hot_path_log_b = true;
            }
            return 0;
        }

        if (_resampler) {
            uint64_t required_input = 0;
            if (ma_linear_resampler_get_required_input_frame_count(_resampler->get(), frame_count, &required_input) != MA_SUCCESS) {
                return 0;
            }
            _pull_buf.resize(static_cast<size_t>(required_input) * _ch_in);
            uint32_t got = _upstream->pull(_pull_buf.data(), static_cast<uint32_t>(required_input));
            if (got == 0) return 0;

            if (_ch_converter) {
                _convert_buf.resize(static_cast<size_t>(got) * _ch_out);
                convertChannels(_ch_converter, _convert_buf.data(), _pull_buf.data(), got);
                uint64_t in_frames = got, out_frames = frame_count;
                resample(_resampler, _convert_buf.data(), &in_frames, data, &out_frames);
                return static_cast<uint32_t>(out_frames);
            } else {
                uint64_t in_frames = got, out_frames = frame_count;
                resample(_resampler, _pull_buf.data(), &in_frames, data, &out_frames);
                return static_cast<uint32_t>(out_frames);
            }
        }

        if (_ch_converter) {
            _pull_buf.resize(static_cast<size_t>(frame_count) * _ch_in);
            uint32_t got = _upstream->pull(_pull_buf.data(), frame_count);
            if (got == 0) return 0;
            convertChannels(_ch_converter, data, _pull_buf.data(), got);
            return got;
        }

        return _upstream->pull(data, frame_count);
    }

    void push(float* data, uint32_t frame_count) {
        if (_downstream == nullptr) {
            if (!_hot_path_log_b) {
                spdlog::error("{} | if(_downstream == nullptr)", FUNC_SIG);
                _hot_path_log_b = true;
            }
            return;
        }

        if (_resampler) {
            const uint64_t expected_out = static_cast<uint64_t>(frame_count) * _sr_out / _sr_in + 1;
            _push_buf.resize(static_cast<size_t>(expected_out) * _ch_out);

            uint64_t in_frames = frame_count;
            uint64_t out_frames = expected_out;

            if (_ch_converter) {
                _convert_buf.resize(static_cast<size_t>(frame_count) * _ch_out);
                convertChannels(_ch_converter, _convert_buf.data(), data, static_cast<uint64_t>(frame_count));
                resample(_resampler, _convert_buf.data(), &in_frames, _push_buf.data(), &out_frames);
            } else {
                resample(_resampler, data, &in_frames, _push_buf.data(), &out_frames);
            }
            _downstream->push(_push_buf.data(), static_cast<uint32_t>(out_frames));
            return;
        }

        if (_ch_converter) {
            _push_buf.resize(static_cast<size_t>(frame_count) * _ch_out);
            convertChannels(_ch_converter, _push_buf.data(), data, static_cast<uint64_t>(frame_count));
            _downstream->push(_push_buf.data(), frame_count);
            return;
        }

        _downstream->push(data, frame_count);
    }

};


} // alo::audio::node

