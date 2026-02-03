#include "miniaudio.h"
#include "spdlog/spdlog.h"

#include <stdio.h>
#include <audio/Engine/engine.hpp>
#include <audio/device/device.hpp>
#include <audio/node/sink_node.hpp>

int main(int argc, char** argv){
    using namespace alo::audio;

    spdlog::set_level(spdlog::level::info);

    Engine engine;
    engine.init();
    engine.start();

    const auto& src_ids = engine.device.ids<DeviceType::SRC>();
    const auto& src_device = engine.device.get<DeviceType::SRC>(src_ids[0]);

    const auto& sink_ids = engine.device.ids<DeviceType::SRC>();
    const auto& sink_device = engine.device.get<DeviceType::SRC>(sink_ids[0]);


    const int N = 2;

    auto [pipeline, id] = pipeline::make();

    const auto& src_container = pipeline.connect(SrcNode{src_device}).split(N);
    const auto& src_container = pipeline->SrcNode(src_device)->split(N);
    auto& nodes = src_container[0]->ReverbNode()->split(N);
    // do same this with src_container[1].

    nodes[0]->GainNode(10amp) -> MixerNode(mixer);
    nodes[1]->GainNode(3amp) -> MixerNode(mixer);

    mixer->SinkNode{sink_device}

    

    engine.pipeline.store(id, std::move(pipeline));


    engine.pipeline.build(id);
    engine.pipeline.start(id);
    engine.pipeline.stop(id);
    engine.pipeline.destroy(id);


    //pipeline->src(src);
    //pipeline->src(sink);
    //pipeline.effect(reverb, ApplyOn::pull);
    //engine.pipeline(pipeline);

    /*
    audio::pipeline audio_pileline; 
    auto mic = audio::node<audio::src::Mic>();
    auto reverb = new audio::node<audio::effect::Reverb>();
    auto recorder = new audio::node<audio::sink::FileRecorder>("out.wav");
    
    auto end_of_chain = mic->connect(reverb)->connect(recorder);
    
    audio_pipeline.build(end_of_chain);
    
    sound_engine.setPipeline(audio_pileline)
    
    sound_engine.start();
    */

    
    /*
    aloh_media::App app(sound_manager);
    app.start();
    
    V1:
    showOnline - return users that are online
    call(user) - make a call to the user
     if in call {
         toggle mic
         toggle video
         leave
        }
        
    */
    //V2:
    // create/join room

    //ma_result result;
    //ma_encoder_config encoderConfig;
    //ma_encoder encoder;
    //ma_device_config deviceConfig;
    //ma_device device;
//
    //if (argc < 2) {
    //    printf("No output file.\n");
    //    return -1;
    //}
//
    //encoderConfig = ma_encoder_config_init(ma_encoding_format_wav, ma_format_f32, 2, 44100);
//
    //if (ma_encoder_init_file(argv[1], &encoderConfig, &encoder) != MA_SUCCESS) {
    //    printf("Failed to initialize output file.\n");
    //    return -1;
    //}
//
    //auto data_callback = [](ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
    //{
    //    ma_encoder_write_pcm_frames((ma_encoder*)pDevice->pUserData, pInput, frameCount, NULL);
//
    //    (void)pOutput;
    //};
//
    //deviceConfig = ma_device_config_init(ma_device_type_duplex);
    //deviceConfig.capture.format   = encoder.config.format;
    //deviceConfig.capture.channels = encoder.config.channels;
    //deviceConfig.sampleRate       = encoder.config.sampleRate;
    //deviceConfig.dataCallback     = data_callback;
    //deviceConfig.pUserData        = &encoder;
//
    //result = ma_device_init(NULL, &deviceConfig, &device);
    //if (result != MA_SUCCESS) {
    //    printf("Failed to initialize capture device.\n");
    //    return -2;
    //}
//
    //result = ma_device_start(&device);
    //if (result != MA_SUCCESS) {
    //    ma_device_uninit(&device);
    //    printf("Failed to start device.\n");
    //    return -3;
    //}
//
    printf("Press Enter to exit...\n");
    getchar();
    //
    //ma_device_uninit(&device);
    //ma_encoder_uninit(&encoder);

    return 0;
}