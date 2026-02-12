#include "Pipeline.hpp"

#include <atomic>
#include <memory>
#include <string>
#include <tuple>
#include <utility>

namespace alo::audio {

Node& Pipeline::connect(Node node) {
    _node_container.emplace_back(std::move(node));
    
    // If there was a previous node, connect it to the new one
    if (_node_container.size() > 1) {
        Node& last_node = _node_container[_node_container.size() - 2];
        Node& current_node = _node_container.back();
        last_node.connect(&current_node);
    }
    
    return _node_container.back();
}

void Pipeline::split(size_t num) {
    // Minimal no-op.
}

void Pipeline::build() {
    for(auto& node : _node_container){
        node.build();
    }
    // Minimal no-op.
}

void Pipeline::start() {
    auto& sink = _node_container.back();
    sink.start();
}

std::tuple<std::unique_ptr<Pipeline>, pipeline_id_t> pipeline::make() {
    static std::atomic_uint64_t counter{0};

    auto p = std::make_unique<Pipeline>();
    auto id = std::string("pipeline-") + std::to_string(counter.fetch_add(1, std::memory_order_relaxed));

    return {std::move(p), std::move(id)};
}

} // namespace alo::audio