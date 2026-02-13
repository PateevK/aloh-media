#pragma once

#include <memory>
#include <utility>
#include <concepts>

namespace alo::audio{

struct Node;
template<typename T>
concept NodeConcept = requires (T node, Node* next, float* data, uint32_t frame_count) {
    { node.connect(next) } -> std::same_as<void>;
    { node.build() } -> std::same_as<void>;
    { node.start() } -> std::same_as<void>;
    { node.pull(data, frame_count) } -> std::convertible_to<uint32_t>;
};

class Node{

    struct NodeI {
        virtual ~NodeI() = default;    
        virtual void connect(Node* next) = 0;
        virtual void start() = 0;
        virtual void build() = 0;
        virtual uint32_t pull(float* data, uint32_t frame_count) = 0;
    };

    template<typename NodeT>
    class NodeModel : public NodeI {
        static_assert(NodeConcept<NodeT>, "NodeT does not satisfy NodeConcept");
    public:
        NodeModel(NodeT node)
        : _node(std::move(node)) {}

        void connect(Node* next) override {
            _node.connect(next);
        }
        
        void start() override {
            _node.start();
        }

        void build() override {
            _node.build();
        }

        uint32_t pull(float* data, uint32_t frame_count) override {
            return _node.pull(data, frame_count);
        }

    private:
        NodeT _node;
    };

    std::unique_ptr<NodeI> pimpl = nullptr;

public:

    template<NodeConcept NodeT>
    Node(NodeT node) 
    : pimpl(std::make_unique<NodeModel<NodeT>>(std::move(node))){

        static_assert(NodeConcept<NodeT>, "NodeT does not satisfy NodeConcept");
    }

    void connect(Node* next) {
        pimpl->connect(next);
    }
        
    void start() {
        pimpl->start();
    }

    void build(){
        pimpl->build();
    }

    uint32_t pull(float* data, uint32_t frame_count){
        return pimpl->pull(data, frame_count);
    }

};

} // alo::audio











