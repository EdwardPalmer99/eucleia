/**
 * @file AnyNode.cpp
 * @author Edward Palmer
 * @date 2025-05-05
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "AnyNode.hpp"
#include "Exceptions.hpp"


AnyNode::AnyNode(Evaluate &&evaluate_)
    : _evaluate(std::move(evaluate_))
{
}


BaseObject *AnyNode::doEvaluate(Scope &scope_) const
{
    if (!_evaluate)
        ThrowException("Cannot call 'evaluate'. Function not set");

    return _evaluate(scope_);
}
