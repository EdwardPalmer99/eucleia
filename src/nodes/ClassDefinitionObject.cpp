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
                                             std::vector<AddVariableNode::Ptr> variableDefs_,
                                             std::vector<FunctionNode::Ptr> methodDefs_)
    : StructDefinitionObject(std::move(typeName_), std::move(parentTypeName_), std::move(variableDefs_)),
      methodDefs(std::move(methodDefs_))
{
    setType(NodeType::ClassDefinition);
}


BaseObject::Ptr ClassDefinitionObject::evaluate(Scope &scope)
{
    // NB: override method defined in StructDefinitionObject.
    if (active)
    {
        ThrowException(typeName + " is already defined");
    }

    active = true;

    // TODO: - would be nice to have single method we override form StructDefinitionObject
    // and then we can call base method and just extend it.
    buildVariableDefHashMap(scope);
    buildMethodDefsHashMap(scope);

    // NB: scope cannot manage lifetime of this definition currently since it
    // is owned by the AST.
    scope.linkObject(typeName, shared_from_this());
    return shared_from_this();
}


void ClassDefinitionObject::installMethodsInScope(Scope &scope) const
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


void ClassDefinitionObject::buildMethodDefsHashMap(const Scope &scope)
{
    if (!allMethodDefsMap.empty())
    {
        return;
    }

    auto parent = std::static_pointer_cast<ClassDefinitionObject>(lookupParent(scope));
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