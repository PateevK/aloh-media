#pragma once

#include <string>

#include <audio/node/node.hpp>

namespace alo::audio{
    
    using pipeline_id_t = std::string;

    class Pipeline{
    public:

    void connect(Node node);
    
    void split(size_t num);
    void build();
    
    private:
    };

    namespace pipeline{
        std::tuple<std::unique_ptr<Pipeline>, pipeline_id_t> make();
    }

}; // alo::audio