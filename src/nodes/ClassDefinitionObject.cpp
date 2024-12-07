/**
 * @file ClassDefinitionObject.cpp
 * @author Edward Palmer
 * @date 2024-11-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "ClassDefinitionObject.hpp"

ClassDefinitionObject::ClassDefinitionObject(std::string typeName_,
                                             std::string parentTypeName_,
                                             std::vector<AddVariableNode *> variableDefs_,
                                             std::vector<FunctionNode *> methodDefs_)
    : StructDefinitionObject(std::move(typeName_), std::move(parentTypeName_), std::move(variableDefs_)),
      methodDefs(std::move(methodDefs_))
{
}


ClassDefinitionObject::~ClassDefinitionObject()
{
    // We are responsible for deleting nodes.
    for (auto *ptr : methodDefs)
    {
        delete ptr;
    }

    methodDefs.clear();
}


BaseObject *ClassDefinitionObject::evaluate(Scope &scope)
{
    // NB: override method defined in StructDefinitionObject.
    if (active)
    {
        EucleiaError("%s is already defined", typeName.c_str());
    }

    active = true;

    // TODO: - would be nice to have single method we override form StructDefinitionObject
    // and then we can call base method and just extend it.
    buildVariableDefHashMap(scope);
    buildMethodDefsHashMap(scope);

    // TODO: - scope should take ownership.
    scope.linkObject(typeName, this);
    return this;
}


void ClassDefinitionObject::installMethodsInScope(Scope &scope) const
{
    for (auto &[name, funcNode] : allMethodDefsMap)
    {
        (void)funcNode->evaluate(scope);
    }
}


void ClassDefinitionObject::buildMethodDefsHashMap(const Scope &scope)
{
    if (!allMethodDefsMap.empty())
    {
        return;
    }

    ClassDefinitionObject *parent = static_cast<ClassDefinitionObject *>(lookupParent(scope));
    if (parent)
    {
        ClassDefinitionObject *parentObj = static_cast<ClassDefinitionObject *>(parent);

        parentObj->buildMethodDefsHashMap(scope); // Unnecessary since to be installed, this will already have happened.

        // Copy parent methods.
        allMethodDefsMap = parentObj->allMethodDefsMap;
    }

    // Now we add our own methods and replace any existing methods with same name
    // We are not going to be too smart initially. We just replace methods with
    // the same name even with different numbers and types of arguments.
    for (FunctionNode *funcDef : methodDefs)
    {
        allMethodDefsMap[funcDef->funcName->name] = funcDef;
    }
}