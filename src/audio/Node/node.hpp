#pragma once

#include <memory>
#include <utility>
#include <concepts>

namespace alo::audio{

class Node;
template<typename T>
concept NodeImplConcept = requires (T node, Node* upstream, Node* downstream, Node* self, float* data, uint32_t frame_count) {
    { node.connect(upstream, downstream) } -> std::same_as<void>;
    { node.build(self) } -> std::same_as<void>;
    { node.start() } -> std::same_as<void>;
    { node.stop() } -> std::same_as<void>;
    { node.pull(data, frame_count) } -> std::convertible_to<uint32_t>;
    { node.push(data, frame_count) } -> std::same_as<void>;
    { node.channels() } -> std::convertible_to<uint32_t>;
    { node.sample_rate() } -> std::convertible_to<uint32_t>;
};

class Node{

    struct NodeI {
        virtual ~NodeI() = default;    
        virtual void connect(Node* upstream, Node* downstream) = 0;
        virtual void start() = 0;
        virtual void build(Node* self) = 0;
        virtual void stop() = 0;
        virtual uint32_t pull(float* data, uint32_t frame_count) = 0;
        virtual void push(float* data, uint32_t frame_count) = 0;
        virtual uint32_t channels() const  = 0;
        virtual uint32_t sample_rate() const = 0;
    };

    template<typename NodeT>
    class NodeModel : public NodeI {
        static_assert(NodeImplConcept<NodeT>, "NodeT does not satisfy NodeConcept");
    public:
        NodeModel(NodeT node)
        : _node(std::move(node)) {}

        void connect(Node* upstream, Node* downstream) override {
            _node.connect(upstream, downstream);
        }
        
        void start() override {
            _node.start();
        }

        void build(Node* self) override {
            _node.build(self);
        }

        void stop() override {
            _node.stop();
        }

        uint32_t pull(float* data, uint32_t frame_count) override {
            return _node.pull(data, frame_count);
        }

        void push(float* data, uint32_t frame_count) override {
            return _node.push(data, frame_count);
        }

        uint32_t channels() const override { return _node.channels(); }
        uint32_t sample_rate() const override { return _node.sample_rate(); }

    private:
        NodeT _node;
    };

    std::unique_ptr<NodeI> pimpl = nullptr;

public:

    template<NodeImplConcept NodeT>
    Node(NodeT node) 
    : pimpl(std::make_unique<NodeModel<NodeT>>(std::move(node))){

        static_assert(NodeImplConcept<NodeT>, "NodeT does not satisfy NodeConcept");
    }

    void connect(Node* upstream, Node* downstream) {
        pimpl->connect(upstream, downstream);
    }
        
    void start() {
        pimpl->start();
    }

    void build(){
        pimpl->build(this);
    }

    void stop(){
        pimpl->stop();
    }

    uint32_t pull(float* data, uint32_t frame_count){
        return pimpl->pull(data, frame_count);
    }

    void push(float* data, uint32_t frame_count){
        pimpl->push(data, frame_count);
    }

    uint32_t channels() const { return pimpl->channels(); }
    uint32_t sample_rate() const { return pimpl->sample_rate(); }

};

} // alo::audio











