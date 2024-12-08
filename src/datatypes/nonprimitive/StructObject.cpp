/**
 * @file StructObject.cpp
 * @author Edward Palmer
 * @date 2024-11-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "StructObject.hpp"
#include "FloatObject.hpp"
#include "IntObject.hpp"
#include "Scope.hpp"
#include "StringObject.hpp"
#include <cassert>

StructObject::StructObject(std::string typeName_, std::string name_)
    : typeName(std::move(typeName_)), name(std::move(name_))
{
}


BaseObject *StructObject::evaluate(Scope &scope)
{
    if (active)
    {
        EucleiaError("StructObject named '%s' of type '%s' is already active",
                     name.c_str(), typeName.c_str());
    }

    active = true;

    // Initialize our instance from the struct definition defined in the scope.
    structDefinition = scope.getNamedObject<StructDefinitionObject>(typeName);
    structDefinition->installVariablesInScope(instanceScope, variableNames);

    // Add the active struct instance to the scope. TODO: - transfer ownership
    // to the scope. Will have to remove this class from AST to do this correctly.
    scope.linkObject(name, this);
    return this;
}


StructObject &StructObject::operator=(const BaseObject &other)
{
    if (this == &other)
    {
        return (*this);
    }

    // 1. cast to struct object.
    const StructObject &otherStruct = other.castObject<StructObject>();

    // 2. check that both are instances of the same struct type.
    if (otherStruct.structDefinition != structDefinition)
    {
        EucleiaError("attempting to assign different struct types.");
    }

    // 3. iterate over the objects stored in the scopes and assign.
    for (auto &variableName : variableNames)
    {
        BaseObject *thisObject = instanceScope.getNamedObject(variableName);
        BaseObject *otherObject = otherStruct.instanceScope.getNamedObject(variableName);

        // Attempt an assignment. Will fail if different types.
        (*thisObject) = (*otherObject);
    }

    return (*this);
}
