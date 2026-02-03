#pragma once

#include "audio/Pipeline/Pipeline.hpp"
#include <memory>
#include <unordered_map>
#include <optional>
// Forward

namespace alo::audio {
    
    using pipeline_handle_t = std::unique_ptr<Pipeline>;
    using err_t = int;
    struct Engine;
    class PipelineM {
    public:
        PipelineM() = delete;
        PipelineM(PipelineM& other) = delete;
        PipelineM(PipelineM&& other) = delete;
        PipelineM operator=(PipelineM& other) = delete;
        PipelineM operator=(PipelineM&& other) = delete;
    
        PipelineM(Engine* e) : engine_(e) {}
        
        void store(pipeline_id_t id, pipeline_handle_t pipeline);
        std::optional<err_t> build(const pipeline_id_t& id);
        void start(const pipeline_id_t& id);
        void stop(const pipeline_id_t& id);
        void destroy(const pipeline_id_t& id);

    private:
        std::unordered_map<pipeline_id_t, pipeline_handle_t> _id_to_pipeline;
        Engine* engine_;
    };


}; // alo::audio