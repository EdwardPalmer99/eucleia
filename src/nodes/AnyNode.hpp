/**
 * @file AnyNode.hpp
 * @author Edward Palmer
 * @date 2025-05-05
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "BaseObject.hpp"
#include "Scope.hpp"
#include <functional>

class AnyNode
{
public:
    using Evaluate = std::function<BaseObject *(Scope &scope_)>;

    AnyNode() = default;
    AnyNode(Evaluate &&evaluate_);

    virtual ~AnyNode() = default;

    inline void setEvaluate(Evaluate &&evaluate_);

    [[nodiscard]] BaseObject *operator()(Scope &scope_) const
    {
        return doEvaluate(scope_);
    }

    template <typename TObject>
    [[nodiscard]] TObject *operator()(Scope &scope_) const
    {
        return static_cast<TObject *>(doEvaluate(scope_));
    }

protected:
    [[nodiscard]] BaseObject *doEvaluate(Scope &scope_) const;

private:
    Evaluate _evaluate{nullptr};
};


void AnyNode::setEvaluate(Evaluate &&evaluate_)
{
    _evaluate = std::move(evaluate_);
}
