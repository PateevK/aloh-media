#pragma once

#include <concepts>
#include <memory>
#include <utility>

namespace alo::audio{

template <typename T>
concept NodeConcept = requires (T node) {
{node.connect()} -> std::same_as<void>;
{node.pull()} -> std::same_as<void>;
{node.push()} -> std::same_as<void>;
{node.build()} -> std::same_as<void>;
{node.start()} -> std::same_as<void>;
};  

class Node{

    struct NodeI {
    
        virtual ~NodeI() = default;
    
        virtual void connect() const = 0;
    
    };

    template< NodeConcept NodeT >
    class NodeModel : public NodeI {
    public:
        NodeModel(NodeT node)
        : _node( std::move(node)) {}

        void connect() const override {
            _node.connect();
        }

    private:
        NodeT _node;
    };

    std::unique_ptr<NodeI> pimpl = nullptr;
public:

    template<NodeConcept NodeT>
    Node(NodeT node) 
    : pimpl(std::make_unique<NodeModel<NodeT>>(std::move(node))){}

    void connect() { 
        pimpl->connect();
    }
};

} // alo::audio