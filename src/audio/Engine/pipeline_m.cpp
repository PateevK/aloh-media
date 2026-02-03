#include "pipeline_m.hpp"
#include "audio/Pipeline/Pipeline.hpp"
#include "audio/Pipeline/Pipeline.hpp"
#include <optional>

namespace alo::audio {

    void PipelineM::store(pipeline_id_t id, pipeline_handle_t pipeline){
        _id_to_pipeline.emplace(std::move(id), std::move(pipeline));
    }

    std::optional<err_t> PipelineM::build(const pipeline_id_t& id){
        auto res = _id_to_pipeline.find(id);
        if(res == _id_to_pipeline.end()){
            return 1;
        }

        const auto& pipeline = res->second;

        pipeline->build();

        return std::nullopt;
    }

    void PipelineM::start(const pipeline_id_t& id){
        
    }

    void PipelineM::stop(const pipeline_id_t& id){

    }

    void PipelineM::destroy(const pipeline_id_t& id){

    }


} // alo::audio