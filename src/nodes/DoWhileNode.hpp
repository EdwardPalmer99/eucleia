/**
 * @file DoWhileNode.hpp
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

class DoWhileNode : public BaseNode
{
public:
    DoWhileNode(BaseNode *condition_, BaseNode *body_)
        : condition(condition_),
          body(body_)
    {
    }

    ~DoWhileNode() override
    {
        delete condition;
        delete body;
    }

    // Evaluate a do-while loop in current scope. Returns nullptr.
    BaseObject *evaluate(Scope &scope) override;


    BaseNode *condition{nullptr};
    BaseNode *body{nullptr};
};
