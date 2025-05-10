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
#include "BaseObject.hpp"
#include "NodeType.hpp"
#include "Scope.hpp"
#include <functional>
#include <memory>

/* Generic node */
class AnyNode : public BaseNode
{
public:
    using Ptr = std::shared_ptr<AnyNode>;

    using EvaluateFunction = std::function<BaseObject *(Scope &)>;

    explicit AnyNode(NodeType type, EvaluateFunction &&evaluateFunc)
        : _type{type}, _evaluateFunc(std::move(evaluateFunc)) {}

    BaseObject *evaluate(Scope &scope) override
    {
        return _evaluateFunc(scope);
    }

    NodeType type() const { return _type; } /* TODO: - move into base class */

private:
    NodeType _type{NodeType::Unknown};
    EvaluateFunction _evaluateFunc;
};