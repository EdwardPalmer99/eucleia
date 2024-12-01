/**
 * @file ClassDefinitionNode.cpp
 * @author Edward Palmer
 * @date 2024-11-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "ClassDefinitionNode.hpp"
#include "ClassDefinitionObject.hpp"

ClassDefinitionNode::ClassDefinitionNode(std::string typeName_,
                                         std::vector<AddVariableNode *> variableDefs_,
                                         std::vector<FunctionNode *> methodDefs_)
    : StructDefinitionNode(typeName_, variableDefs_),
      methodDefs(std::move(methodDefs_))
{
}


ClassDefinitionNode::~ClassDefinitionNode()
{
    if (evaluateCalled)
        return;

    for (FunctionNode *node : methodDefs)
    {
        delete node;
    }

    methodDefs.clear();
}


BaseObject *ClassDefinitionNode::evaluate(Scope &scope)
{
    if (evaluateCalled)
    {
        EucleiaError("evaluate method already called for ClassDefinitionNode.");
    }

    evaluateCalled = true;

    ClassDefinitionObject *obj = scope.createManagedObject<ClassDefinitionObject>(std::move(variableDefs), std::move(methodDefs));
    scope.linkObject(typeName, obj);

    return obj;
}
