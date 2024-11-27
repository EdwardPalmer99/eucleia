/**
 * @file IfNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "IfNode.hpp"
#include "ExpressionScope.hpp"
#include "IntObject.hpp"

BaseObject *IfNode::evaluate(Scope &scope)
{
    // NB: the condition should be evaluated in its own scope as it is within ()
    // brackets. We don't want it to persist once evaluated.
    if (evaluateExpression<BoolObject>(ifCondition, scope).value)
        return thenDo->evaluate(scope);
    else if (elseDo != nullptr) // Optional "else"
        return elseDo->evaluate(scope);
    else
        return nullptr;
}