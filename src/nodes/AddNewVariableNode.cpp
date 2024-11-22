/**
 * @file AddNewVariableNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "AddNewVariableNode.hpp"

BaseObject *AddNewVariableNode::evaluate(Scope &scope)
{
    BaseObject *objectPtr = nullptr;

    switch (variableType)
    {
        case Int:
        case Bool:
            objectPtr = scope.createManagedObject<IntObject>();
            break;
        case Float:
            objectPtr = scope.createManagedObject<FloatObject>();
            break;
        case String:
            objectPtr = scope.createManagedObject<StringObject>();
            break;
        case Array:
            objectPtr = scope.createManagedObject<ArrayObject>();
            break;
        // TODO: - handle function type. (will need to think about this.)
        default:
            printEucleiaError("%s", "cannot create a variable of specified type.");
    }

    scope.defineObject(variableName, objectPtr); // TODO: - could call this linkObjectToVariableName()
    return objectPtr;
}


/// Type checking.
bool AddNewVariableNode::passesAssignmentTypeCheck(const BaseObject &assignObject) const
{
    switch (variableType)
    {
        case Int:
            return assignObject.isObjectType<IntObject>();
        case Float:
            return assignObject.isObjectType<FloatObject>();
        case Bool:
            return assignObject.isObjectType<BoolObject>();
        case String:
            return assignObject.isObjectType<StringObject>();
        case Array:
            return assignObject.isObjectType<ArrayObject>();
        default:
            return false;
    }
}


std::string AddNewVariableNode::description() const
{
    switch (variableType)
    {
        case Bool:
            return "Bool";
        case Int:
            return "Int";
        case Float:
            return "Float";
        case String:
            return "String";
        case Array:
            return "Array";
        default:
            return "Unknown";
    }
}