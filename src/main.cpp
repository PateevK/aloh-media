#include "miniaudio.h"
#include "spdlog/spdlog.h"

#include <print>
#include <stdio.h>
#include <audio/Engine/Engine.hpp>
#include <audio/device/device.hpp>
#include "audio/Utils/ring_buffer.hpp"

int main(int argc, char** argv){
    spdlog::set_level(spdlog::level::info);

    auto res = alo::audio::utils::RingBuffer<alo::audio::utils::RingBufferType::PCM>::make(2, 2);
    if(!res){
        return 0;
    }  

    const auto& rb = res.value();


    alo::audio::Engine engine;
    engine.init();
    engine.start();

    const auto& ids = engine.device_m().ids<alo::audio::DeviceType::SRC>();

    const auto& device = engine.device_m().device<alo::audio::DeviceType::SRC>(ids[0]);

    device->do_shit();


    //auto src_id = engine.srcId();
    //auto sink_id = engine.sinkId();
    //const auto& src_device = engine.device(src_id);
    //const auto& sink_device = engine.device(sink_id);
//
    //auto* src = new alo::audio::node<alo::audio::NodeType::src>(src_device);
    //auto* sink = new alo::audio::node<alo::audio::NodeType::sink>(sink_device);
    //auto* reverb = new alo::audio::node<alo::audio::NodeType::reverb>(sink_device);
//
    //auto* pipeline = new alo::audio::Pipeline;
//
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