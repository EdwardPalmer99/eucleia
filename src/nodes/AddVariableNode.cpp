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

AddVariableNode::AddVariableNode(std::string name, AnyObject::Type type)
    : LookupVariableNode(std::move(name)),
      _variableType(type)
{
    setType(NodeType::AddVariable);
}

AnyObject::Ptr AddVariableNode::evaluate(Scope &scope)
{
    /* TODO: - add support for functions (to enable passing to other functions, etc) */
    auto objectPtr = ObjectFactory::allocate(_variableType);
    scope.linkObject(name(), objectPtr);

    return objectPtr;
}


/// Type checking.
bool AddVariableNode::passesAssignmentTypeCheck(const AnyObject &assignObject) const
{
    return assignObject.isType(_variableType);
}


std::string AddVariableNode::description() const
{
    switch (_variableType)
    {
        case AnyObject::Bool:
            return "Bool";
        case AnyObject::Int:
            return "Int";
        case AnyObject::Float:
            return "Float";
        case AnyObject::String:
            return "String";
        case AnyObject::Array:
            return "Array";
        default:
            return "Unknown";
    }
}


AddReferenceVariableNode::AddReferenceVariableNode(std::string referenceName_,
                                                   std::string boundName_,
                                                   AnyObject::Type boundType_)
    : AddVariableNode(boundName_, boundType_),
      referenceName(referenceName_)
{
}


AnyObject::Ptr AddReferenceVariableNode::evaluate(Scope &scope)
{
    // 1. Lookup the object associated with the variable name defined in this
    // scope or a parent scope (no issue with lifetimes such as to be bound
    // object going out of scope before our reference.
    AnyObject::Ptr boundObject = scope.getNamedObject(name());

    // TODO: - this will not work for classes/structs since they could point to different types.
    // 2. Type checking. The type of the reference must match that of the bound object.
    if (!passesAssignmentTypeCheck(*boundObject))
    {
        ThrowException("Cannot bind reference " + referenceName + " to variable " + name() + ". Types do not match!");
    }

    // 3. Instead of creating a new object, we add the reference name and link
    // to this existing object in the scope.
    scope.linkObject(referenceName, boundObject);

    return boundObject;
}