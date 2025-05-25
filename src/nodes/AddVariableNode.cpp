/**
 * @file AddVariableNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "AddVariableNode.hpp"
#include "Exceptions.hpp"
#include "ObjectFactory.hpp"
#include "Scope.hpp"


AddVariableNode::AddVariableNode(std::string name, AnyObject::Type type)
    : LookupVariableNode(std::move(name)),
      _variableType(type)
{
    setType(NodeType::AddVariable);
}


AnyObject::Ref AddVariableNode::evaluateRef(Scope &scope)
{
    /* Construct an empty object of that type and add to the scope. We assume user will set value with copy assignment */
    return scope.link(name(), ObjectFactory::createEmptyObject(_variableType));
}


AnyObject AddVariableNode::evaluate(Scope &scope)
{
    return evaluateRef(scope);
}


/// Type checking.
bool AddVariableNode::passesAssignmentTypeCheck(const AnyObject &assignObject) const
{
    return assignObject.isType(_variableType);
}


AddReferenceVariableNode::AddReferenceVariableNode(std::string referenceName_,
                                                   std::string boundName_,
                                                   AnyObject::Type boundType_)
    : AddVariableNode(boundName_, boundType_),
      referenceName(referenceName_)
{
}


AnyObject::Ref AddReferenceVariableNode::evaluateRef(Scope &scope)
{
    // 1. Lookup the object associated with the variable name defined in this scope or a parent scope (no issue with
    // lifetimes such as to be bound object going out of scope before our reference.
    AnyObject::Ref boundObject = scope.getObjectRef(name());

    // TODO: - this will not work for classes/structs since they could point to different types.
    // 2. Type checking. The type of the reference must match that of the bound object.
    if (!passesAssignmentTypeCheck(boundObject))
    {
        ThrowException("Cannot bind reference [" + referenceName + "] to variable [" + name() + "]. Types do not match!");
    }

    // 3. Instead of creating a new object, we add the reference name and link to this existing object in the scope.
    return scope.alias(referenceName, name());
}


AnyObject AddReferenceVariableNode::evaluate(Scope &scope)
{
    return evaluateRef(scope); /* Copy result */
}