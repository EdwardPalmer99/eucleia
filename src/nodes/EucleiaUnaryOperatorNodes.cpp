//
//  EucleiaUnaryOperatorNodes.cpp
//  eucleia
//
//  Created by Edward on 04/02/2024.
//

#include "EucleiaUnaryOperatorNodes.hpp"

std::shared_ptr<BaseObject> NotNode::evaluate(Scope &scope)
{
    auto bodyEvaluated = body->evaluate<BoolObject>(scope);

    return std::make_shared<BoolObject>(!bodyEvaluated->value);
}


std::shared_ptr<BaseObject> NegationNode::evaluate(Scope &scope)
{
    auto bodyEvaluated = body->evaluate(scope);

    if (bodyEvaluated->isObjectType<IntObject>())
        return std::make_shared<IntObject>(-bodyEvaluated->castObject<IntObject>());
    else if (bodyEvaluated->isObjectType<FloatObject>())
        return std::make_shared<FloatObject>(-bodyEvaluated->castObject<FloatObject>());
    else
        printWarpError("%s", "invalid object type.");
}


std::shared_ptr<BaseObject> PrefixIncrementNode::evaluate(Scope &scope)
{
    // 1. Body should be an already-declared variable.
    assert(body->type() == BaseNode::VariableName);
    auto variableNameNode = std::static_pointer_cast<VariableNameNode>(body);

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


std::shared_ptr<BaseObject> PrefixDecrementNode::evaluate(Scope &scope)
{
    // 1. Body should be an already-declared variable.
    assert(body->type() == BaseNode::VariableName);
    auto variableNameNode = std::static_pointer_cast<VariableNameNode>(body);

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
