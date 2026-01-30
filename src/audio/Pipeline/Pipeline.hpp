#pragma once

#include <audio/Node/node.hpp>
#include <vector>

namespace alo{

namespace audio{
    
    class Pipeline{
    public:

    auto src(Node src);
    const auto& src() const;

    auto sink(Node sink);
    const auto& sink() const;

    // Append an effect to be applyed after all effects already appended.
    auto effect(Node node);

    private:
        Node _src;        
        Node _sink;        
        std::vector<Node> _effect_vec;        
    };

} // audio

} // alo