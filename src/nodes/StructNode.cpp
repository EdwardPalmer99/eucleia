/**
 * @file StructNode.cpp
 * @author Edward Palmer
 * @date 2024-11-24
 *
 * @copyright Copyright (c) 2024
 *
 */


#include "StructNode.hpp"
#include "AnyObject.hpp"
#include "ObjectFactory.hpp"
#include "Scope.hpp"
#include <cassert>
#include <iostream>


StructNode::StructNode(std::string typeName_, std::string name_)
    : typeName(std::move(typeName_)), name(std::move(name_))
{
}


AnyObject StructNode::evaluate(Scope &scope)
{
    if (active)
    {
        ThrowException("StructNode named " + name + " of type " + typeName + " is already active");
    }

    active = true;

    // Initialize our instance from the struct definition defined in the scope.
    structDefinition = std::static_pointer_cast<StructDefinitionNode>(scope.getObjectRef(typeName).getValue<BaseNode::Ptr>());
    structDefinition->installVariablesInScope(_instanceScope, variableNames);

    // Add the active struct instance to the scope. TODO: - transfer ownership
    // to the scope. Will have to remove this class from AST to do this correctly.
    return scope.link(name, AnyObject(shared_from_this(), AnyObject::Struct));
}


StructNode &StructNode::operator=(const StructNode &other)
{
    if (this == &other)
    {
        return (*this);
    }

    // 1. check that both are instances of the same struct type.
    if (other.structDefinition != structDefinition)
    {
        ThrowException("cannot assign Struct objects with different definitions");
    }

    // 2. iterate over the objects stored in the scopes and assign.
    for (auto &variableName : variableNames)
    {
        AnyObject::Ref thisObject = _instanceScope.getObjectRef(variableName);
        AnyObject::Ref otherObject = other._instanceScope.getObjectRef(variableName);

        // Attempt an assignment. Will fail if different types.
        thisObject = otherObject;
    }

    return (*this);
}
