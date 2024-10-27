//
//  EucleiaUnaryOperatorNodes.cpp
//  eucleia
//
//  Created by Edward on 04/02/2024.
//

#include "EucleiaUnaryOperatorNodes.hpp"

std::shared_ptr<BaseObject> NotNode::evaluate(Scope &scope)
{
    auto bodyEvaluated = body->evaluate(scope);
    assert(bodyEvaluated->type() == BaseObject::ObjectType::Bool);

    return std::make_shared<BoolObject>(!bodyEvaluated->boolCast());
}


std::shared_ptr<BaseObject> NegationNode::evaluate(Scope &scope)
{
    auto bodyEvaluated = body->evaluate(scope);

    return bodyEvaluated->negateValue();
}


std::shared_ptr<BaseObject> PrefixIncrementNode::evaluate(Scope &scope)
{
    // 1. Body should be an already-declared variable.
    assert(body->type() == BaseNode::VariableName);
    auto variableNameNode = std::static_pointer_cast<VariableNameNode>(body);

    // 2. Object associated with variable name in scope must be an integer.
    auto bodyEvaluated = body->evaluate(scope);

    // 3. Increment object (throws error if not supported on type).
    bodyEvaluated->incrementValue();

    return bodyEvaluated;
}


std::shared_ptr<BaseObject> PrefixDecrementNode::evaluate(Scope &scope)
{
    // 1. Body should be an already-declared variable.
    assert(body->type() == BaseNode::VariableName);
    auto variableNameNode = std::static_pointer_cast<VariableNameNode>(body);

    // 2. Object associated with variable name in scope must be an integer.
    auto bodyEvaluated = body->evaluate(scope);

    // 3. Increment object (throws error if not supported on type).
    bodyEvaluated->decrementValue();

    return bodyEvaluated;
}
