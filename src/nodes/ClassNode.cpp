/**
 * @file ClassNode.cpp
 * @author Edward Palmer
 * @date 2024-11-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "ClassNode.hpp"
#include "AnyObject.hpp"
#include "FunctionNode.hpp"
#include "ObjectFactory.hpp"
#include <cassert>


ClassNode::ClassNode(std::string typeName_, std::string name_)
    : StructNode(std::move(typeName_), std::move(name_))
{
}

AnyObject::Ptr ClassNode::evaluate(Scope &scope)
{
    // TODO: - inefficient, should have another method we can call to do most of StructNode::evaluate.
    if (active)
    {
        ThrowException("ClassNode named " + name + " of type " + typeName + " is already active");
    }

    active = true;

    // Initialize our instance from the struct definition defined in the scope.
    auto theObject = scope.getNamedObject(typeName);

    structDefinition = std::static_pointer_cast<StructDefinitionNode>(theObject->getValue<BaseNode::Ptr>());
    structDefinition->installVariablesInScope(_instanceScope, variableNames);

    auto classDefinition = std::static_pointer_cast<ClassDefinitionNode>(structDefinition);
    classDefinition->installMethodsInScope(_instanceScope);

    // Add the active struct instance to the scope. TODO: - transfer ownership
    // to the scope. Will have to remove this class from AST to do this correctly.
    auto wrappedClass = ObjectFactory::allocate(shared_from_this(), AnyObject::Class);

    scope.linkObject(name, wrappedClass);
    return wrappedClass;
}