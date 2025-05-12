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
#include "PropertyInterface.hpp"
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
        : BaseNode(type), _evaluateFunc(std::move(evaluateFunc)) {}

    BaseObject *evaluate(Scope &scope) final
    {
        return _evaluateFunc(scope);
    }

private:
    EvaluateFunction _evaluateFunc;
};


class AnyPropertyNode : public AnyNode, public PropertyInterface
{
public:
    using Ptr = std::shared_ptr<AnyPropertyNode>;

    explicit AnyPropertyNode(NodeType type, EvaluateFunction &&evaluateFunc, EvaluateFunction &&evaluateNoCloneFunc)
        : AnyNode(type, std::move(evaluateFunc)),
          _evaluateNoCloneFunc(std::move(evaluateNoCloneFunc)) {}

    BaseObject *evaluateNoClone(Scope &scope) final
    {
        return _evaluateNoCloneFunc(scope);
    }

private:
    EvaluateFunction _evaluateNoCloneFunc;
};