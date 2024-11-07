//
//  EucleiaUnaryOperatorNodes.cpp
//  eucleia
//
//  Created by Edward on 04/02/2024.
//

#include "EucleiaUnaryOperatorNodes.hpp"

BaseObject *NotNode::evaluate(Scope &scope)
{
    auto bodyEvaluated = body->evaluate<BoolObject>(scope);

    return scope.createManagedObject<BoolObject>(!(*bodyEvaluated));
}


BaseObject *NegationNode::evaluate(Scope &scope)
{
    auto bodyEvaluated = body->evaluate(scope);

    if (bodyEvaluated->isObjectType<IntObject>())
        return scope.createManagedObject<IntObject>(-bodyEvaluated->castObject<IntObject>());
    else if (bodyEvaluated->isObjectType<FloatObject>())
        return scope.createManagedObject<FloatObject>(-bodyEvaluated->castObject<FloatObject>());
    else
        printWarpError("%s", "invalid object type.");
}


BaseObject *PrefixIncrementNode::evaluate(Scope &scope)
{
    // 1. Body should be an already-declared variable.
    assert(body->type() == BaseNode::VariableName);

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
        printWarpError("%s", "cannot use prefix operator on object of type.\n");
    }
}


BaseObject *PrefixDecrementNode::evaluate(Scope &scope)
{
    // 1. Body should be an already-declared variable.
    assert(body->type() == BaseNode::VariableName);

    // 2. Object associated with variable name in scope must be integer or float.
    auto bodyEvaluated = body->evaluate(scope);
    if (bodyEvaluated->isObjectType<IntObject>())
    {
        --(bodyEvaluated->castObject<IntObject>());
        return bodyEvaluated;
    }
    else if (bodyEvaluated->isObjectType<FloatObject>())
    {
        --(bodyEvaluated->castObject<FloatObject>());
        return bodyEvaluated;
    }
    else
    {
        printWarpError("%s", "cannot use prefix operator on object of type.\n");
    }
}
