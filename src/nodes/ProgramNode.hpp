/**
 * @file ProgramNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseNode.hpp"
#include "BaseObject.hpp"
#include "Scope.hpp"
#include <cassert>
#include <vector>

class ProgramNode : public BaseNode
{
public:
    ProgramNode(std::vector<BaseNode::Ptr> nodes_) : programNodes(std::move(nodes_))
    {
        setType(NodeType::Program);
    }

    ~ProgramNode() override = default;

    // Ownership of nodes passes to the caller.
    std::vector<BaseNode::Ptr> releaseNodes()
    {
        auto returnedNodes = programNodes;
        programNodes.clear();

        return returnedNodes;
    }

    BaseNode::Ptr operator[](size_t index) const
    {
        assert(index < programNodes.size());
        return programNodes[index];
    }

    // Evaluates a vector of nodes sequentially. Returns nullptr.
    BaseObject::Ptr evaluate(Scope &scope) override;

    std::vector<BaseNode::Ptr> programNodes;
};