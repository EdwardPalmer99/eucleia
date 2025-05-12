/**
 * @file ClassAccessNode.cpp
 * @author Edward Palmer
 * @date 2024-12-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "ClassAccessNode.hpp"
#include "ClassDefinitionObject.hpp"
#include "ClassObject.hpp"


ClassMethodCallNode::ClassMethodCallNode(std::string instanceName_, FunctionCallNode *methodCallNode_)
    : instanceName(instanceName_),
      methodCallNode(methodCallNode_)
{
    setType(NodeType::ClassMethodCall);
}


ClassMethodCallNode::~ClassMethodCallNode()
{
    delete methodCallNode;
}


BaseObject *ClassMethodCallNode::evaluate(Scope &scope)
{
    ClassObject *thisObject = scope.getNamedObject<ClassObject>(instanceName);

    // Important: to correctly evaluate the method, we need to add a parent scope
    // for the class instance temporarily each time we evaluate so function has
    // access to stuff created outside class.
    thisObject->instanceScope.setParentScope(&scope);

    BaseObject *result = methodCallNode->evaluate(thisObject->instanceScope);

    // Set back to avoid problems if we forget to reset it in future.
    thisObject->instanceScope.setParentScope(nullptr);

    return result;
}