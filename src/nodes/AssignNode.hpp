/**
 * @file AssignNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseNode.hpp"
#include "Scope.hpp"

class AssignNode : public BaseNode
{
public:
    AssignNode(BaseNode *left_, BaseNode *right_)
        : left(left_),
          right(right_)
    {
    }

    ~AssignNode() override
    {
        delete left;
        delete right;
    }

    BaseObject *evaluate(Scope &scope) override;

    BaseObject *evaluateArrayAccess(Scope &scope);

    BaseNode *left{nullptr};
    BaseNode *right{nullptr};
};
