/**
 * @file StructDefinitionNode.cpp
 * @author Edward Palmer
 * @date 2024-11-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "StructDefinitionNode.hpp"
#include "AddVariableNode.hpp"
#include "EucleiaUtility.hpp"
#include "StructDefinitionObject.hpp"

StructDefinitionNode::~StructDefinitionNode()
{
    // Already transferred ownership of vector.
    if (evaluateCalled)
        return;

    for (AddVariableNode *obj : variableDefs)
    {
        delete obj;
    }

    variableDefs.clear();
}


BaseObject *StructDefinitionNode::evaluate(Scope &scope)
{
    if (evaluateCalled)
    {
        EucleiaError("evaluate method already called for StructDefinitionNode.");
    }

    evaluateCalled = true;

    StructDefinitionObject *parentObj = parentTypeName.empty() ? nullptr : scope.getNamedObject<StructDefinitionObject>(parentTypeName);

    StructDefinitionObject *obj = scope.createManagedObject<StructDefinitionObject>(std::move(variableDefs), parentObj);
    scope.linkObject(typeName, obj);

    return obj;
}