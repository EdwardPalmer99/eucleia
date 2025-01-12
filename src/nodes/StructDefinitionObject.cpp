/**
 * @file StructDefinitionObject.cpp
 * @author Edward Palmer
 * @date 2024-11-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "StructDefinitionObject.hpp"


StructDefinitionObject::StructDefinitionObject(std::string typeName_,
                                               std::string parentTypeName_,
                                               std::vector<AddVariableNode *> variableDefs_)
    : typeName(std::move(typeName_)),
      parentTypeName(std::move(parentTypeName_)),
      variableDefs(std::move(variableDefs_))
{
}


StructDefinitionObject::~StructDefinitionObject()
{
    // We are responsible for deleting nodes.
    for (auto *ptr : variableDefs)
    {
        delete ptr;
    }

    variableDefs.clear();
}

StructDefinitionObject *StructDefinitionObject::lookupParent(const Scope &scope) const
{
    if (parentTypeName.empty())
    {
        return nullptr;
    }

    return scope.getNamedObject<StructDefinitionObject>(parentTypeName);
}


BaseObject *StructDefinitionObject::evaluate(Scope &scope)
{
    if (active) // Expect one definition only!
    {
        ThrowException(typeName + " is already defined");
    }

    // Definition is now active.
    active = true;

    // Locate parent if there is one and setup everything. We cannot do this before
    // as the parent will not have been installed in a scope to look for. If
    // something goes wrong, this will be a runtime error as we cannot do anything
    // prior to this.
    buildVariableDefHashMap(scope);

    // NB: scope cannot manage lifetime of this definition currently since it
    // is owned by the AST. TODO: - rectify this.
    scope.linkObject(typeName, this);
    return this;
}


void StructDefinitionObject::installVariablesInScope(Scope &scope, std::unordered_set<std::string> &variableNames) const
{
    if (!active)
    {
        ThrowException("the struct definition is inactive!");
    }

    variableNames.clear();

    for (auto iter = allVariableDefsMap.begin(); iter != allVariableDefsMap.end(); ++iter)
    {
        variableNames.insert(iter->first);
        (void)iter->second->evaluate(scope);
    }
}


void StructDefinitionObject::buildVariableDefHashMap(const Scope &scope)
{
    if (!allVariableDefsMap.empty())
    {
        return; // Already built.
    }

    StructDefinitionObject *parent = lookupParent(scope);
    if (parent)
    {
        parent->buildVariableDefHashMap(scope); // TODO: - unnecessary

        // Copy parent definitions.
        allVariableDefsMap = parent->allVariableDefsMap;
    }

    // Now we add our own variable definitions and check for any clashes.
    for (AddVariableNode *variableDef : variableDefs)
    {
        auto iter = allVariableDefsMap.find(variableDef->name);
        if (iter != allVariableDefsMap.end())
        {
            ThrowException("duplicate class variable " + iter->first);
        }

        allVariableDefsMap[variableDef->name] = variableDef;
    }
}
