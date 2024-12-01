/**
 * @file NotNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "NotNode.hpp"
#include "IntObject.hpp"

BaseObject *NotNode::evaluate(Scope &scope)
{
    auto bodyEvaluated = body->evaluate<BoolObject>(scope);

    return scope.createManagedObject<BoolObject>(!(*bodyEvaluated));
}