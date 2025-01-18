/**
 * @file NegationNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "NegationNode.hpp"
#include "FloatObject.hpp"
#include "IntObject.hpp"

BaseObject *NegationNode::evaluate(Scope &scope)
{
    auto bodyEvaluated = body->evaluate(scope);

    if (bodyEvaluated->isObjectType<IntObject>())
        return scope.createManagedObject<IntObject>(-bodyEvaluated->castObject<IntObject>());
    else if (bodyEvaluated->isObjectType<FloatObject>())
        return scope.createManagedObject<FloatObject>(-bodyEvaluated->castObject<FloatObject>());
    else
        ThrowException("invalid object type");
}