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
    : typeName(std::move(typeName_)), name(std::move(name_))
{
}

AnyObject::Ptr ClassNode::evaluate(Scope &scope)
{
    if (classDefinition)
    {
        ThrowException("Class [" + name + "] is already initialized!");
    }

    // Initialize our instance from the struct definition defined in the scope.
    auto theObject = scope.getNamedObject(typeName);

    classDefinition = std::static_pointer_cast<ClassDefinitionNode>(theObject->getValue<BaseNode::Ptr>());

    classDefinition->installVariablesInScope(_instanceScope);
    classDefinition->installMethodsInScope(_instanceScope);

    // Add the active struct instance to the scope. TODO: - transfer ownership
    // to the scope. Will have to remove this class from AST to do this correctly.
    auto wrappedClass = ObjectFactory::allocate(shared_from_this(), AnyObject::Class);

    scope.linkObject(name, wrappedClass);
    return wrappedClass;
}


ClassNode &ClassNode::operator=(const ClassNode &other)
{
    if (this == &other)
    {
        return (*this);
    }

    // 1. check that both are instances of the same struct type.
    if (other.classDefinition != classDefinition)
    {
        ThrowException("cannot assign Class objects with different definitions");
    }

    // 2. Copy scope.
    _instanceScope = other._instanceScope;
    return (*this);
}