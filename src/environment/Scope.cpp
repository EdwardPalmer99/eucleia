/**
 * @file Scope.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "Scope.hpp"
#include "AnyObject.hpp"
#include "Exceptions.hpp"
#include "Logger.hpp"
#include <cassert>
#include <functional>
#include <iostream>


Scope::Scope(const Scope &parentScope)
    : _enclosingScope(const_cast<Scope *>(&parentScope))
{
}


AnyObject *Scope::getObjectPtr(const VariableName &name) const
{
    auto iter = _objectPtrMap.find(name);
    if (iter != _objectPtrMap.end())
    {
        return const_cast<AnyObject *>(iter->second); // TODO: - bit dodgy with const_cast
    }

    // Otherwise check if it is defined in our parent's scope? Keep working outwards.
    if (_enclosingScope)
    {
        return _enclosingScope->getObjectPtr(name);
    }

    return nullptr;
}

#include "Logger.hpp"

AnyObject &Scope::getObjectRef(const VariableName &name) const
{
    log().info("Getting object reference for name " + name);
    auto ptr = getObjectPtr(name);

    if (ptr)
    {
        log().debug("the pointer type is: <" + ptr->typeToString() + ">");

        return *ptr;
    }

    // Not defined.
    ThrowException("No variable defined in scope with name [" + name + "]");
}


void Scope::checkForNameClashesInCurrentScope(const VariableName &name) const
{
    if (!_objectPtrMap.count(name))
        return;

    ThrowException("Variable [" + name + "] is already defined in current scope");
}


AnyObject::Ref Scope::alias(const VariableName &nameAlias, const VariableName &name)
{
    checkForNameClashesInCurrentScope(nameAlias);

    auto *object = getObjectPtr(name);
    if (!object)
    {
        ThrowException("No variable defined in scope with name [" + name + "]");
    }

    _objectPtrMap[nameAlias] = object;

    return std::ref(*object);
}


AnyObject::Ref Scope::link(const VariableName &name, AnyObject &&object)
{
    log().debug("Adding variable '" + name + "' with type '" + object.typeToString() + "'");

    assert(!name.empty() && object.getType() != AnyObject::NotSet);

    // 1. Check for name clashes. This is where we have two variables with the same name defined in the SAME scope.
    // Note that it's okay to have the same variable defined multiple times if they're in different scopes --
    // this is 'variable shadowing'.
    checkForNameClashesInCurrentScope(name);

    // 2. Add to map. This will ensure that we now ignore any outer-scope variables with this name (variable shadowing).
    _objects.push_back(object);

    _objectPtrMap[name] = &_objects.back();

    return _objects.back();
}
