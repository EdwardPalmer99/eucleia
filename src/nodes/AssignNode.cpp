/**
 * @file AssignNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "AssignNode.hpp"
#include "ArrayAccessNode.hpp"

BaseObject *AssignNode::evaluate(Scope &scope)
{
    // Special case; example: array[0] = 3;
    if (left->isNodeType<ArrayAccessNode>())
    {
        return evaluateArrayAccess(scope);
    }

    // 1. Cast LHS to a variable node or a variable name node.
    assert(left->isNodeType<AddNewVariableNode>() || left->isNodeType<LookupVariableNode>());

    // Evaluate the LHS.
    // Case 1: VariableName node --> returns shared pointer to existing object (not useful).
    // Case 2: Variable node --> creates new object in scope.
    if (left->isNodeType<AddNewVariableNode>())
    {
        (void)left->evaluate(scope);
    }

    // 2. Evaluate the right-hand-side.
    auto rightEvaluated = right->evaluate(scope);

    // 3. Update value of variable object.
    auto &leftVariableName = left->castNode<LookupVariableNode>();
    scope.updateObject(leftVariableName.variableName, rightEvaluated);

    return nullptr;
}

/// Special method called if LHS is an array accessor.
BaseObject *AssignNode::evaluateArrayAccess(Scope &scope)
{
    assert(left->isNodeType<ArrayAccessNode>());

    // 1. Get the array object at that index.
    auto currentValue = left->evaluate(scope);

    // 2. Evaluate RHS to get new value for the object.
    auto newValue = right->evaluate(scope);

    // 3. Check that the types match.
    assert(currentValue->typesMatch(*newValue));

    // Set to new value.
    *currentValue = *newValue;
    printEucleiaError("%s", "WIP: LOGIC IS NOT CORRECT. THINK ABOUT HOW TO REMOVE OLD VALUE FROM SCOPE.");

    return nullptr;
}