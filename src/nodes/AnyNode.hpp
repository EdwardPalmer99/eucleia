/**
 * @file AnyNode.hpp
 * @author Edward Palmer
 * @date 2025-05-07
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "AnyObject.hpp"
#include "BaseNode.hpp"
#include "Scope.hpp"
#include <functional>
#include <memory>
#include <optional>


/* Generic node */
class AnyNode : public BaseNode
{
public:
    using Ptr = std::shared_ptr<AnyNode>;

    using EvaluateFunction = std::function<AnyObject(Scope &)>;

    explicit AnyNode(NodeType type, EvaluateFunction &&evaluateFunc)
        : BaseNode(type), _evaluateFunc(std::move(evaluateFunc)) {}

    class AnyObject evaluate(Scope &scope) final;

private:
    EvaluateFunction _evaluateFunc;
};


class AnyPropertyNode : public AnyNode
{
public:
    using Ptr = std::shared_ptr<AnyPropertyNode>;
    using EvaluateRefFunction = std::function<AnyObject &(Scope &)>;

    explicit AnyPropertyNode(NodeType type, EvaluateFunction &&evaluateFunc, EvaluateRefFunction &&evaluateRefFunc)
        : AnyNode(type, std::move(evaluateFunc)),
          _evaluateRefFunc(std::move(evaluateRefFunc)) {}

    class AnyObject &evaluateRef(Scope &scope) final;

private:
    EvaluateRefFunction _evaluateRefFunc;
};