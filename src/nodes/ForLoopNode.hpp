/**
 * @file ForLoopNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseNode.hpp"
#include "EucleiaScope.hpp"

class ForLoopNode : public BaseNode
{
public:
    ForLoopNode(BaseNode *start_, BaseNode *condition_, BaseNode *update_, BaseNode *body_)
        : start(start_),
          condition(condition_),
          update(update_),
          body(body_)
    {
    }

    ~ForLoopNode() override
    {
        delete start;
        delete condition;
        delete update;
        delete body;
    }

    // Evaluates a for-loop in current scope. Returns nullptr.
    BaseObject *evaluate(Scope &scope) override;


    BaseNode *start{nullptr};
    BaseNode *condition{nullptr};
    BaseNode *update{nullptr};
    BaseNode *body{nullptr};
};
