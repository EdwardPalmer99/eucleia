/**
 * @file Scope.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "Scope.hpp"
#include "EucleiaUtility.hpp"

Scope::Scope(const Scope &_parent)
    : Scope(&_parent)
{
}

Scope::~Scope()
{
    for (BaseObject *obj : objectsCreatedInScope)
    {
        delete obj;
    }
}

Scope::Scope(const Scope *_parent)
    : parent(_parent)
{
}


BaseObject *Scope::getOptionalNamedObject(const std::string &name) const
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


BaseObject *Scope::getNamedObject(const std::string &name) const
{
    BaseObject *obj = getOptionalNamedObject(name);
    if (!obj)
    {
        printEucleiaError("undefined variable '%s'.\n", name.c_str());
    }

    return obj;
}


bool Scope::hasNamedObject(const std::string &name) const
{
    return (getOptionalNamedObject(name) != nullptr);
}


void Scope::linkObject(const std::string &name, BaseObject *object)
{
    assert(object != nullptr);

    // 1. Check for name clashes. This is where we have two variables with
    // the same name defined in the SAME scope.
    auto iter = linkedObjectForName.find(name);
    if (iter != linkedObjectForName.end())
    {
        printEucleiaError("%s is already defined in current scope.", name.c_str());
    }

    // 2. Add to map. This will ensure that we now ignore any outer-scope variables
    // with this name (variable shadowing).
    linkedObjectForName[name] = object;
}


void Scope::updateLinkedObject(const std::string &name, BaseObject *object)
{
    assert(object != nullptr);

    // 1. Call moving-up scopes to the parent and update in the first scope where
    // the object is defined.
    auto iter = linkedObjectForName.find(name);
    if (iter == linkedObjectForName.end())
    {
        // Not found. Try in parent scope.
        if (parent)
        {
            return const_cast<Scope *>(parent)->updateLinkedObject(name, object);
        }
        else
        {
            // Not found!!
            printEucleiaError("cannot update variable %s - not found in any scope.\n", name.c_str());
        }
    }

    // Found in this scope. Great.
    // 2. Check types of existing object matches that of new object.
    BaseObject *existingObject = iter->second;
    if (!existingObject->typesMatch((*object)))
    {
        printEucleiaError("Setting object of type %s with incompatible type %s",
                          existingObject->typeName().c_str(), object->typeName().c_str());
    }

    // 3. Update the mapping in this scope.
    linkedObjectForName[name] = object;

    // 4. If we created existingObject in OUR scope then we should delete it
    // as it is no longer in use (r-value).
    objectsCreatedInScope.erase(existingObject);
    delete existingObject;
}
