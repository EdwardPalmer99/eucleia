/**
 * @file StructDefinitionNode.cpp
 * @author Edward Palmer
 * @date 2024-11-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "StructDefinitionNode.hpp"
#include "AddVariableNode.hpp"
#include "AnyObject.hpp"
#include "ObjectFactory.hpp"
#include "Scope.hpp"

StructDefinitionNode::StructDefinitionNode(std::string typeName_,
                                           std::string parentTypeName_,
                                           std::vector<AddVariableNode::Ptr> variableDefs_)
    : typeName(std::move(typeName_)),
      parentTypeName(std::move(parentTypeName_)),
      variableDefs(std::move(variableDefs_))
{
    setType(NodeType::StructDefinition);
}


std::shared_ptr<StructDefinitionNode> StructDefinitionNode::lookupParent(const Scope &scope) const
{
    if (parentTypeName.empty())
    {
        return nullptr;
    }

    AnyObject::Ref theObject = scope.getObjectRef(parentTypeName);

    return std::static_pointer_cast<StructDefinitionNode>(theObject.getValue<BaseNode::Ptr>());
}


AnyObject StructDefinitionNode::evaluate(Scope &scope)
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

    /* NB: need to wrap-up in an object shared pointer */
    auto objectWrapper = AnyObject(shared_from_this(), AnyObject::_StructDefinition);

    return scope.link(typeName, std::move(objectWrapper));
}


void StructDefinitionNode::installVariablesInScope(Scope &scope, std::unordered_set<std::string> &variableNames) const
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


void StructDefinitionNode::buildVariableDefHashMap(const Scope &scope)
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
