#pragma once

#include <memory>
#include <utility>

namespace alo::audio{

class Node{

    struct NodeI {
        virtual ~NodeI() = default;    
        virtual void connect(Node* next) = 0;
        virtual void start() = 0;
        virtual void build() = 0;
    };

    template<typename NodeT>
    class NodeModel : public NodeI {
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

    private:
        NodeT _node;
    };

    std::unique_ptr<NodeI> pimpl = nullptr;

public:

    template<typename NodeT>
    Node(NodeT node) 
    : pimpl(std::make_unique<NodeModel<NodeT>>(std::move(node))){}

    void connect(Node* next) {
        pimpl->connect(next);
    }
        
    void start() {
        pimpl->start();
    }

    void build(){
        pimpl->build();
    }

};

} // alo::audio











