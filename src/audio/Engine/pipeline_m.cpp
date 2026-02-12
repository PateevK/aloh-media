#include "pipeline_m.hpp"
#include "audio/Pipeline/Pipeline.hpp"
#include "audio/Pipeline/Pipeline.hpp"
#include <optional>

#include <spdlog/spdlog.h>

namespace alo::audio {

    void PipelineM::store(pipeline_id_t id, pipeline_handle_t pipeline){
        _id_to_pipeline.emplace(id, std::move(pipeline));
        spdlog::debug("{} | pipeline with id: was added to engine", FUNC_SIG, id);
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

    std::optional<err_t> PipelineM::start(const pipeline_id_t& id){
        const auto& pipeline_it = _id_to_pipeline.find(id);        
        if(pipeline_it == _id_to_pipeline.end()){
            return 1; 
        }
        const auto& pipeline = pipeline_it->second;

        pipeline->start();

        return {};
    }

    void PipelineM::stop(const pipeline_id_t& id){

    }

    void PipelineM::destroy(const pipeline_id_t& id){

    }


} // alo::audio