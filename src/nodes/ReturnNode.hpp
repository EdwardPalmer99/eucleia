/**
 * @file ReturnNode.hpp
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


class ReturnNode : public BaseNode
{
public:
    ReturnNode(BaseNode *returnValue_ = nullptr)
        : returnNode(returnValue_)
    {
    }

    ~ReturnNode() override
    {
        delete returnNode;
    }

    BaseObject *evaluate(Scope &scope) override;

    BaseNode *returnNode{nullptr};
};