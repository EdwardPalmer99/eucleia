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
#include "StructNode.hpp"
#include <iostream>

BaseObject *AssignNode::evaluate(Scope &scope)
{
    // Setting array or struct values.
    if (left->isNodeType<ArrayAccessNode>())
    {
        ArrayAccessNode &accessor = left->castNode<ArrayAccessNode>();

        *(accessor.evaluateNoClone(scope)) = *(right->evaluate(scope));
        return nullptr;
    }
    else if (left->isNodeType<StructAccessNode>())
    {
        StructAccessNode &accessor = left->castNode<StructAccessNode>();

        *(accessor.evaluateNoClone(scope)) = *(right->evaluate(scope));
        std::cout << "StructNodeAccess... " << std::endl;
        std::cout << *(right->evaluate(scope)) << std::endl;
        return nullptr;
    }

    // TODO: - don't actually need to update the linked object here. We can just
    // perform an assignment as above. More efficient!!

    // 1. Cast LHS to a variable node or a variable name node.
    assert(left->isNodeType<AddVariableNode>() || left->isNodeType<LookupVariableNode>());

    // Evaluate the LHS.
    // Case 1: VariableName node --> returns shared pointer to existing object (not useful).
    // Case 2: Variable node --> creates new object in scope.
    if (left->isNodeType<AddVariableNode>())
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
