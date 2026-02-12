#include "Pipeline.hpp"

#include <atomic>
#include <memory>
#include <string>
#include <tuple>
#include <utility>

namespace alo::audio {

Node* Pipeline::connect(Node node) {

    if(_node_container.empty()){
        _node_container.emplace_back(std::make_unique<Node>(std::move(node)));
        return _node_container.back().get();
    }

    auto& prev_node = _node_container.back();
    _node_container.emplace_back(std::make_unique<Node>(std::move(node)));
    auto& current_node = _node_container.back();
    
    prev_node->connect(current_node.get());
    current_node->connect(prev_node.get());
    
    return current_node.get();
}

void Pipeline::split(size_t num) {
    // Minimal no-op.
}

void Pipeline::build() {
    for(auto& node : _node_container){
        node->build();
    }
    // Minimal no-op.
}

void Pipeline::start() {
      for(auto& node : _node_container){
        node->start();
    }
}

std::tuple<std::unique_ptr<Pipeline>, pipeline_id_t> pipeline::make() {
    static std::atomic_uint64_t counter{0};

    auto p = std::make_unique<Pipeline>();
    auto id = std::string("pipeline-") + std::to_string(counter.fetch_add(1, std::memory_order_relaxed));

    return {std::move(p), std::move(id)};
}

} // namespace alo::audio