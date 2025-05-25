/**
 * @file FunctionNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseNode.hpp"
#include "FunctionCallNode.hpp"
#include <string>


class FunctionNode : public FunctionCallNode, public std::enable_shared_from_this<FunctionNode>
{
public:
    using Ptr = std::shared_ptr<FunctionNode>;

    FunctionNode(std::string funcName_, BaseNodePtrVector funcArgs_, BaseNode::Ptr funcBody_)
        : FunctionCallNode(std::move(funcName_), std::move(funcArgs_)),
          funcBody(funcBody_)
    {
        setType(NodeType::Function);
    }

    ~FunctionNode() override = default;

    class AnyObject evaluate(class Scope &scope) override;

    BaseNode::Ptr funcBody{nullptr};
};
