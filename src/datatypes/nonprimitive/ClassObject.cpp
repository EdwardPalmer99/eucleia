/**
 * @file ClassObject.cpp
 * @author Edward Palmer
 * @date 2024-11-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "ClassObject.hpp"
#include "FunctionNode.hpp"
#include <cassert>

ClassObject::ClassObject(std::string typeName_, std::string name_)
    : StructObject(std::move(typeName_), std::move(name_))
{
}

BaseObject *ClassObject::evaluate(Scope &scope)
{
    // TODO: - inefficient, should have another method we can call to do most of StructObject::evaluate.
    if (active)
    {
        EucleiaError("ClassObject named '%s' of type '%s' is already active",
                     name.c_str(), typeName.c_str());
    }

    active = true;

    // Initialize our instance from the struct definition defined in the scope.
    structDefinition = scope.getNamedObject<StructDefinitionObject>(typeName);
    structDefinition->installVariablesInScope(instanceScope, variableNames);

    auto classDefinition = static_cast<ClassDefinitionObject *>(structDefinition);
    classDefinition->installMethodsInScope(instanceScope);

    // Add the active struct instance to the scope. TODO: - transfer ownership
    // to the scope. Will have to remove this class from AST to do this correctly.
    scope.linkObject(name, this);
    return this;
}
