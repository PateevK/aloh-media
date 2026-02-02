#include "Pipeline.hpp"

namespace alo{

namespace audio{

    auto Pipeline::src(Node src){
        //_src = src;
    }

    //const auto& Pipeline::src() const {
//        return _src;
    //}

    //auto Pipeline::sink(Node sink){
        //_sink = sink;
    //}

    //const auto& Pipeline::sink() const{
  //      return _sink;
    //}

    // Append an effect to be applyed after all effects already appended.
    auto Pipeline::effect(Node node) -> void{
        _effect_vec.emplace_back(std::move(node));
    }

} // audio

} // alo