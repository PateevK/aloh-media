
playback_callback(output){
    for(producer){
        data.add(producer.pull());
    }

    output = data
}

capture_callback(data){
    device.write_ring_buffer(data)
    for(consumer){
        consumer.push(data)
    }
}

void capture_callback(const void* pInput, ma_uint32 frameCount) {
    // Each sink has its OWN dedicated ring buffer
    for (auto& sink : active_sinks) {
        // This is a thread-safe, lock-free SPSC write
        sink->ringBuffer.write(pInput, frameCount);
    }
}

src -> reverb -> output
file -> amplify -> output

                                    output.pull 
                             pull(reverb) * pull(amplify)
                src.push(reverb)                    file.push(amplify)