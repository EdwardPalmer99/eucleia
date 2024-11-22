/**
 * @file IfNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseNode.hpp"
#include "Scope.hpp"

class IfNode : public BaseNode
{
public:
    IfNode(BaseNode *condition_,
           BaseNode *thenDo_,
           BaseNode *elseDo_ = nullptr)
        : ifCondition(condition_),
          thenDo(thenDo_),
          elseDo(elseDo_)
    {
    }

    ~IfNode() override
    {
        delete ifCondition;
        delete thenDo;
        delete elseDo;
    }

    // Evaluate an if/else statement in current scope. Returns nullptr.
    BaseObject *evaluate(Scope &scope) override;


    BaseNode *ifCondition{nullptr};
    BaseNode *thenDo{nullptr};
    BaseNode *elseDo{nullptr}; // Links to next argument (if node!).
};