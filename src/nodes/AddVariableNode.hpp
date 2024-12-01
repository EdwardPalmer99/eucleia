/**
 * @file AddVariableNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseObject.hpp"
#include "LookupVariableNode.hpp"
#include "ObjectTypes.hpp"
#include "Scope.hpp"

class AddVariableNode : public LookupVariableNode
{
public:
    AddVariableNode(std::string name_, ObjectType type_)
        : LookupVariableNode(std::move(name_)),
          type(type_) {}

    // Creates a new empty variable of a given type to the scope (i.e. int a;).
    BaseObject *evaluate(Scope &scope) override;

    std::string description() const;

    //  Type checking for variable assignment.
    bool passesAssignmentTypeCheck(const BaseObject &assignObject) const;

    const ObjectType type;
};
