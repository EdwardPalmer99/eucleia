/**
 * @file ClassDefinitionNode.cpp
 * @author Edward Palmer
 * @date 2024-11-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "ClassDefinitionNode.hpp"
#include "AnyObject.hpp"
#include "Exceptions.hpp"
#include "ObjectFactory.hpp"


ClassDefinitionNode::ClassDefinitionNode(std::string typeName_,
                                         std::string parentTypeName_,
                                         std::vector<AddVariableNode::Ptr> variableDefs_,
                                         std::vector<FunctionNode::Ptr> methodDefs_)
    : typeName(std::move(typeName_)),
      parentTypeName(std::move(parentTypeName_)),
      variableDefs(std::move(variableDefs_)),
      methodDefs(std::move(methodDefs_))
{
    setType(NodeType::ClassDefinition);
}


std::shared_ptr<ClassDefinitionNode> ClassDefinitionNode::lookupParent(const Scope &scope) const
{
    if (parentTypeName.empty())
    {
        return nullptr;
    }

    auto theObject = scope.getNamedObject(parentTypeName);

    return std::static_pointer_cast<ClassDefinitionNode>(theObject->getValue<BaseNode::Ptr>());
}


AnyObject::Ptr ClassDefinitionNode::evaluate(Scope &scope)
{
    if (active)
    {
        ThrowException(typeName + " is already defined");
    }

    active = true;

    buildVariableDefHashMap(scope);
    buildMethodDefsHashMap(scope);

    /* NB: wrap-up in an object shared pointer */
    auto objectWrapper = ObjectFactory::allocate(shared_from_this(), AnyObject::_ClassDefinition);

    scope.linkObject(typeName, objectWrapper);
    return objectWrapper;
}


void ClassDefinitionNode::installVariablesInScope(Scope &scope, std::unordered_set<std::string> &variableNames) const
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


void ClassDefinitionNode::installMethodsInScope(Scope &scope) const
{
    if (!active)
    {
        ThrowException("The class definition is inactive!");
    }

    for (auto &[name, funcNode] : allMethodDefsMap)
    {
        (void)funcNode->evaluate(scope);
    }
}


void ClassDefinitionNode::buildVariableDefHashMap(const Scope &scope)
{
    if (!allVariableDefsMap.empty())
    {
        return; // Already built.
    }

    auto parent = lookupParent(scope);
    if (parent)
    {
        parent->buildVariableDefHashMap(scope); // TODO: - unnecessary

        // Copy parent definitions.
        allVariableDefsMap = parent->allVariableDefsMap;
    }

    // Now we add our own variable definitions and check for any clashes.
    for (AddVariableNode::Ptr &variableDef : variableDefs)
    {
        auto iter = allVariableDefsMap.find(variableDef->name());
        if (iter != allVariableDefsMap.end())
        {
            ThrowException("duplicate class variable " + iter->first);
        }

        allVariableDefsMap[variableDef->name()] = variableDef;
    }
}


void ClassDefinitionNode::buildMethodDefsHashMap(const Scope &scope)
{
    if (!allMethodDefsMap.empty())
    {
        return;
    }

    auto parent = std::static_pointer_cast<ClassDefinitionNode>(lookupParent(scope));
    if (parent)
    {
        parent->buildMethodDefsHashMap(scope); // Unnecessary since to be installed, this will already have happened.

        // Copy parent methods.
        allMethodDefsMap = parent->allMethodDefsMap;
    }

    // Now we add our own methods and replace any existing methods with same name
    // We are not going to be too smart initially. We just replace methods with
    // the same name even with different numbers and types of arguments.
    for (FunctionNode::Ptr funcDef : methodDefs)
    {
        allMethodDefsMap[funcDef->_funcName] = funcDef;
    }
}