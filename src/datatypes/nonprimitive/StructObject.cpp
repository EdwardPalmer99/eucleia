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

StructObject::StructObject(StructDefinitionObject *structDefinition_)
    : structDefinition(structDefinition_)
{
    assert(structDefinition);
    structDefinition->installVariablesInScope(instanceScope, variableNames);
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
