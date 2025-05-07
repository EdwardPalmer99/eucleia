/**
 * @file AnyNode.hpp
 * @author Edward Palmer
 * @date 2025-05-07
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "BaseNode.hpp"
#include "Scope.hpp"
#include <functional>

/* Generic node */
class AnyNode : public BaseNode
{
public:
    using EvaluateFunction = std::function<BaseObject *(Scope &)>;

    explicit AnyNode(EvaluateFunction evaluateFunc) : _evaluateFunc(std::move(evaluateFunc)) {}

    BaseObject *evaluate(Scope &scope) override
    {
        return _evaluateFunc(scope);
    }

private:
    EvaluateFunction _evaluateFunc;
};