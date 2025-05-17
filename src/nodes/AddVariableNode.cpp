/**
 * @file AddVariableNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "AddVariableNode.hpp"
#include "ArrayObject.hpp"
#include "FloatObject.hpp"
#include "IntObject.hpp"
#include "ObjectFactory.hpp"
#include "StringObject.hpp"


BaseObject::Ptr AddVariableNode::evaluate(Scope &scope)
{
    /* TODO: - add support for functions (to enable passing to other functions, etc) */
    auto objectPtr = ObjectFactory::allocate(_variableType);
    scope.linkObject(name(), objectPtr);

    return objectPtr;
}


/// Type checking.
bool AddVariableNode::passesAssignmentTypeCheck(const BaseObject &assignObject) const
{
    switch (_variableType)
    {
        case ObjectType::Int:
            return assignObject.isObjectType<IntObject>();
        case ObjectType::Float:
            return assignObject.isObjectType<FloatObject>();
        case ObjectType::Bool:
            return assignObject.isObjectType<BoolObject>();
        case ObjectType::String:
            return assignObject.isObjectType<StringObject>();
        case ObjectType::Array:
            return assignObject.isObjectType<ArrayObject>();
        default:
            return false;
    }
}


std::string AddVariableNode::description() const
{
    switch (_variableType)
    {
        case ObjectType::Bool:
            return "Bool";
        case ObjectType::Int:
            return "Int";
        case ObjectType::Float:
            return "Float";
        case ObjectType::String:
            return "String";
        case ObjectType::Array:
            return "Array";
        default:
            return "Unknown";
    }
}


AddReferenceVariableNode::AddReferenceVariableNode(std::string referenceName_,
                                                   std::string boundName_,
                                                   ObjectType boundType_)
    : AddVariableNode(boundName_, boundType_),
      referenceName(referenceName_)
{
}


BaseObject::Ptr AddReferenceVariableNode::evaluate(Scope &scope)
{
    // 1. Lookup the object associated with the variable name defined in this
    // scope or a parent scope (no issue with lifetimes such as to be bound
    // object going out of scope before our reference.
    BaseObject::Ptr boundObject = scope.getNamedObject(name());

    // 2. Type checking. The type of the reference must match that of the bound
    // object.
    bool passesTypeChecking{false};

    // TODO: - add type checking for classes and struct references.
    switch (_variableType)
    {
        case ObjectType::Int:
            passesTypeChecking = boundObject->isObjectType<IntObject>();
            break;
        case ObjectType::Float:
            passesTypeChecking = boundObject->isObjectType<FloatObject>();
            break;
        case ObjectType::String:
            passesTypeChecking = boundObject->isObjectType<StringObject>();
            break;
        case ObjectType::Bool:
            passesTypeChecking = boundObject->isObjectType<StringObject>();
            break;
        case ObjectType::Array:
            passesTypeChecking = boundObject->isObjectType<ArrayObject>();
            break;
        case ObjectType::Struct:
        case ObjectType::Class:
        default:
            passesTypeChecking = true;
            break; // No type checking currently!
    }

    if (!passesTypeChecking)
    {
        ThrowException("Cannot bind reference " + referenceName + " to variable " + name() + ". Types do not match!");
    }

    // 3. Instead of creating a new object, we add the reference name and link
    // to this existing object in the scope.
    scope.linkObject(referenceName, boundObject);

    return boundObject;
}