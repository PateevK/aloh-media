#pragma once

#include <memory>
#include <string>
#include <vector>

#include <audio/node/node.hpp>

namespace alo::audio{
    
using pipeline_id_t = std::string;

class Pipeline{
public:    
    Node* connect(Node node);
    
    template<typename NodeT>
    Node* connect(NodeT&& concrete_node) {
        return connect(Node(std::forward<NodeT>(concrete_node)));
    }

    void split(size_t num);
    void build();
    void start();

private:
    using node_container_t = std::vector<std::unique_ptr<Node>>; 
    node_container_t _node_container;

}; // class Pipeline

namespace pipeline{
    std::tuple<std::unique_ptr<Pipeline>, pipeline_id_t> make();
}

}; // alo::audio