#pragma once 

#include <spdlog/spdlog.h>

#include "node_base.hpp"

namespace alo::audio::node{

class Muxer : public NodeBase<Muxer> {

public:

    Muxer() = default;

};


} // alo::audio::node

