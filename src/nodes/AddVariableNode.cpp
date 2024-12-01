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
#include "StringObject.hpp"

BaseObject *AddVariableNode::evaluate(Scope &scope)
{
    BaseObject *objectPtr = nullptr;

    switch (type)
    {
        case ObjectType::Int:
        case ObjectType::Bool:
            objectPtr = scope.createManagedObject<IntObject>();
            break;
        case ObjectType::Float:
            objectPtr = scope.createManagedObject<FloatObject>();
            break;
        case ObjectType::String:
            objectPtr = scope.createManagedObject<StringObject>();
            break;
        case ObjectType::Array:
            objectPtr = scope.createManagedObject<ArrayObject>();
            break;
        // TODO: - handle function type. (will need to think about this.)
        default:
            EucleiaError("cannot create a variable of specified type.");
    }

    scope.linkObject(name, objectPtr); // TODO: - could call this linkObjectToVariableName()
    return objectPtr;
}


/// Type checking.
bool AddVariableNode::passesAssignmentTypeCheck(const BaseObject &assignObject) const
{
    switch (type)
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
    switch (type)
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
