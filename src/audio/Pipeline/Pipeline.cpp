#include "Pipeline.hpp"

#include <atomic>
#include <memory>
#include <string>
#include <tuple>
#include <utility>

namespace alo::audio {

void Pipeline::connect(Node /*node*/) {
    // Minimal no-op.
}

void Pipeline::split(size_t /*num*/) {
    // Minimal no-op.
}

void Pipeline::build() {
    // Minimal no-op.
}

std::tuple<std::unique_ptr<Pipeline>, pipeline_id_t> pipeline::make() {
    static std::atomic_uint64_t counter{0};

    auto p = std::make_unique<Pipeline>();
    auto id = std::string("pipeline-") + std::to_string(counter.fetch_add(1, std::memory_order_relaxed));

    return {std::move(p), std::move(id)};
}

} // namespace alo::audio