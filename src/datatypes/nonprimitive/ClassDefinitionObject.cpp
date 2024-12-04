/**
 * @file ClassDefinitionObject.cpp
 * @author Edward Palmer
 * @date 2024-11-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "ClassDefinitionObject.hpp"

ClassDefinitionObject::ClassDefinitionObject(std::vector<AddVariableNode *> variableDefs_,
                                             std::vector<FunctionNode *> methodDefs_,
                                             ClassDefinitionObject *parent_)
    : StructDefinitionObject(std::move(variableDefs_), parent_),
      methodDefs(std::move(methodDefs_))
{
    buildMethodDefsHashMap();
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


void ClassDefinitionObject::buildMethodDefsHashMap()
{
    if (!allMethodDefsMap.empty())
    {
        return;
    }

    if (parent)
    {
        ClassDefinitionObject *parentObj = static_cast<ClassDefinitionObject *>(parent);

        parentObj->buildMethodDefsHashMap();

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