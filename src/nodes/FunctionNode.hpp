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


class FunctionNode : public FunctionCallNode
{
public:
    FunctionNode(BaseNode *funcName_, BaseNodePtrVector funcArgs_, BaseNode *funcBody_)
        : FunctionCallNode(funcName_, std::move(funcArgs_)),
          funcBody(funcBody_)
    {
    }

    ~FunctionNode() override
    {
        delete funcBody;
    }

    BaseObject *evaluate(Scope &scope) override;

    BaseNode *funcBody{nullptr};
};
