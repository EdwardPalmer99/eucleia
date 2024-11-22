/**
 * @file WhileNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseNode.hpp"
#include "BaseObject.hpp"
#include "DoWhileNode.hpp"
#include "EucleiaScope.hpp"


class WhileNode : public DoWhileNode
{
public:
    WhileNode(BaseNode *_condition, BaseNode *_body)
        : DoWhileNode(_condition, _body)
    {
    }

    // Evaluate a while loop in current scope. Returns nullptr.
    BaseObject *evaluate(Scope &scope) override;
};
