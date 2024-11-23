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
    // NB: - cannot currently handle modification of arrays.
    // i.e. array[0] = 1;

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
    scope.updateLinkedObject(leftVariableName.variableName, rightEvaluated);

    return nullptr;
}
