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
#include "ReferenceObject.hpp"
#include "StructAccessNode.hpp"
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
        return nullptr;
    }
    // Special case of a reference.
    else if (left->isNodeType<AddReferenceVariableNode>())
    {
        // A reference must be bound to another variable so the RHS must be a
        // lookupVariableNode type (an already defined variable).
        assert(right->isNodeType<LookupVariableNode>());

        // We have a reference to the object in the scope! Not a copy!!
        BaseObject *objectRHS = right->evaluate(scope);

        // TODO: - there will be issues if linked object is destroyed first due
        // to scopes.

        // Now we create a reference object and link to the object on the RHS.
        ReferenceObject *objectLHS = left->evaluate<ReferenceObject>(scope);
        objectLHS->linkedObject = objectRHS;

        return nullptr;
    }

    assert(left->isNodeType<AddVariableNode>() || left->isNodeType<LookupVariableNode>());

    // Case 1: AddVariableNode -> we create default init object, add to scope and return.
    // Case 2: LookupVariableNode -> we object defined in scope (not cloned!) - TODO: - think about whether we should clone it.
    BaseObject *objectLHS = left->evaluate(scope);

    // Object we want to assign to LHS.
    // NB: evaluate in temporary scope so destroy when we exit this function.
    Scope tmpScope(scope);
    BaseObject *objectRHS = right->evaluate(tmpScope);

    // Update directly. TODO: - We will need to implement this for some object types still.
    *objectLHS = *objectRHS;
    return nullptr;
}
