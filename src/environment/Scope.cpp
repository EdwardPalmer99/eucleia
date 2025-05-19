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
#include <cassert>

Scope::Scope(const Scope &_parent)
    : Scope(&_parent)
{
}

Scope::Scope(const Scope *_parent)
    : parent(const_cast<Scope *>(_parent))
{
}


AnyObject::Ptr Scope::getOptionalNamedObject(const std::string &name) const
{
    // Try in our scope (to handle variable shadowing).
    auto iter = linkedObjectForName.find(name);
    if (iter != linkedObjectForName.end())
    {
        return (iter->second);
    }

    // Otherwise check if it is defined in our parent's scope? Keep working outwards.
    if (parent)
    {
        return parent->getOptionalNamedObject(name);
    }

    // Not defined.
    return nullptr;
}


AnyObject::Ptr Scope::getNamedObject(const std::string &name) const
{
    AnyObject::Ptr obj = getOptionalNamedObject(name);
    if (!obj)
    {
        ThrowException("undefined variable " + name);
    }

    return obj;
}


bool Scope::hasNamedObject(const std::string &name) const
{
    return (getOptionalNamedObject(name) != nullptr);
}


void Scope::linkObject(const std::string &name, AnyObject::Ptr object)
{
    assert(object != nullptr);

    // 1. Check for name clashes. This is where we have two variables with
    // the same name defined in the SAME scope.
    auto iter = linkedObjectForName.find(name);
    if (iter != linkedObjectForName.end())
    {
        ThrowException(name + " is already defined in current scope");
    }

    // 2. Add to map. This will ensure that we now ignore any outer-scope variables
    // with this name (variable shadowing).
    linkedObjectForName[name] = object;
}
