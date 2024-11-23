/**
 * @file AddNewVariableNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseObject.hpp"
#include "LookupVariableNode.hpp"
#include "Scope.hpp"


class AddNewVariableNode : public LookupVariableNode
{
public:
    enum VariableType // TODO: - don't use an enum. Somehow use a typeid to pass -in the desired object type to create.
    {
        Int,
        Float,
        Bool,
        String,
        Array
        // Function	<-- allow this to be set.
    };

    std::string description() const;

    AddNewVariableNode(std::string variableName_, VariableType variableType_)
        : LookupVariableNode(std::move(variableName_)),
          variableType(variableType_) {}

    // Creates a new empty variable of a given type to the scope (i.e. int a;).
    BaseObject *evaluate(Scope &scope) override;


    //  Type checking for variable assignment.
    bool passesAssignmentTypeCheck(const BaseObject &assignObject) const;

    const VariableType variableType;
};
