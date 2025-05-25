/**
 * @file FunctionCallNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseNode.hpp"
#include <algorithm>
#include <string>
#include <vector>


class FunctionCallNode : public BaseNode
{
public:
    using Ptr = std::shared_ptr<FunctionCallNode>;

    FunctionCallNode(std::string funcName_, BaseNodePtrVector funcArgs_)
        : _funcName(std::move(funcName_)),
          _funcArgs(std::move(funcArgs_))
    {
        setType(NodeType::FunctionCall);
    }

    ~FunctionCallNode() override = default;

    // TODO: - don't forget to do performance profiling for Fib sequence and see memory requirements for old and new version
    // TODO: - create a new PR after this for parser to store all nodes in AST in flat array using pointers with method to delete by walking along array.
    class AnyObject evaluate(class Scope &scope) override;

    class AnyObject evaluateFunctionBody(BaseNode &funcBody, class Scope &funcScope);

    std::string _funcName; /* TODO: - hide */
    BaseNodePtrVector _funcArgs{nullptr};
};
