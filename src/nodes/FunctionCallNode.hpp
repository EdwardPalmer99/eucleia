/**
 * @file FunctionCallNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "AddVariableNode.hpp"
#include "BaseNode.hpp"
#include "BaseObject.hpp"
#include "Scope.hpp"
#include <algorithm>
#include <string>
#include <vector>


class FunctionCallNode : public BaseNode
{
public:
    FunctionCallNode(std::string funcName_, BaseNodePtrVector funcArgs_)
        : _funcName(std::move(funcName_)),
          _funcArgs(std::move(funcArgs_))
    {
        setType(NodeType::FunctionCall);
    }

    ~FunctionCallNode() override
    {
        std::for_each(_funcArgs.begin(), _funcArgs.end(), [](BaseNode *node)
        { delete node; });
    }

    // TODO: - don't forget to do performance profiling for Fib sequence and see memory requirements for old and new version
    // TODO: - create a new PR after this for parser to store all nodes in AST in flat array using pointers with method to delete by walking along array.
    BaseObject *evaluate(Scope &scope) override;

    BaseObject *evaluateFunctionBody(BaseNode &funcBody, Scope &funcScope);

    std::string _funcName;
    BaseNodePtrVector _funcArgs{nullptr};
};
