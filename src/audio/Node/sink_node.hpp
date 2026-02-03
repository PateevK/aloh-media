#pragma once

#include <print>

class SinkNode{
    public:

    void connect() const {
        std::println("connect");
    };

    void push() const {
        std::println("push");
    };
    
    void pull() const {
        std::println("pull");
    };

    void build() const {
        std::println("push");
    };
    
    void start() const {
        std::println("pull");
    };


};