/**
 * @file PrefixIncrementNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "PrefixIncrementNode.hpp"
#include "FloatObject.hpp"
#include "IntObject.hpp"
#include "LookupVariableNode.hpp"
#include <cassert>

BaseObject *PrefixIncrementNode::evaluate(Scope &scope)
{
    // 1. Body should be an already-declared variable.
    assert(body->isNodeType<LookupVariableNode>());

    // 2. Object associated with variable name in scope must be integer or float.
    auto bodyEvaluated = body->evaluate(scope);
    if (bodyEvaluated->isObjectType<IntObject>())
    {
        ++(bodyEvaluated->castObject<IntObject>());
        return bodyEvaluated;
    }
    else if (bodyEvaluated->isObjectType<FloatObject>())
    {
        ++(bodyEvaluated->castObject<FloatObject>());
        return bodyEvaluated;
    }
    else
    {
        ThrowException("cannot use prefix operator on object of type");
    }
}