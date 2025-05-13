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
#include "BaseObject.hpp"
#include "FunctionCallNode.hpp"
#include <string>


class FunctionNode : public FunctionCallNode
{
public:
    FunctionNode(std::string funcName_, BaseNodePtrVector funcArgs_, BaseNode *funcBody_)
        : FunctionCallNode(std::move(funcName_), std::move(funcArgs_)),
          funcBody(funcBody_)
    {
        setType(NodeType::Function);
    }

    ~FunctionNode() override
    {
        delete funcBody;
    }

    BaseObject *evaluate(Scope &scope) override;

    BaseNode *funcBody{nullptr};
};
