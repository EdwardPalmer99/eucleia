//
//  EucleiaScope.cpp
//  Eucleia
//
//  Created by Edward on 14/01/2024.
//

#include "EucleiaScope.hpp"
#include "EucleiaUtility.hpp"

Scope::Scope(const Scope &_parent)
    : Scope(&_parent)
{
}

Scope::~Scope()
{
    for (BaseObject *obj : ownedObjects)
    {
        delete obj;
    }
}

Scope::Scope(const Scope *_parent)
    : parent{_parent}
{
    // If there is a parent, shallow-copy all values. If we modify the value in
    // this scope, it will modify these variables defined in an outer scope.
    if (parent)
    {
        objects = parent->objects;
        objectCreationScope = parent->objectCreationScope;
    }
}


bool Scope::hasObject(const std::string &name) const
{
    return (objects.find(name) != objects.end());
}


bool Scope::objectCreatedInScope(const std::string &name) const
{
    return hasObject(name) && (objectCreationScope.at(name) == this);
}


BaseObject *Scope::getDefinedObject(const std::string &name) const
{
    auto it = objects.find(name);
    if (it == objects.end())
    {
        printWarpError("undefined variable '%s'.\n", name.c_str());
    }

    return (it->second);
}

// TODO: - needs checks. This object must be OWNED by us already. Therefore, we can check in our std::unordered_set whether it exists.
void Scope::defineObject(const std::string &name, BaseObject *object)
{
    // 1. Check for name clashes. This is where we have two variables with
    // the same name defined in the SAME scope.
    checkForVariableNameClash(name);

    // 2. Remove any existing objects with same name. If there are any these will
    // have been defined in an outer scope. This is variable shadowing. We do not
    // have ownership of these outer-scope variables.
    removeObject(name);

    // 3. Set object creation scope.
    objectCreationScope[name] = this;
    objects[name] = object;
}


void Scope::updateObject(const std::string &name, BaseObject *object)
{
    assert(object && hasObject(name));

    // Perform basic type-checking.
    auto existingObject = objects[name];

    // NB: None type means it has not yet been initialized.
    if (existingObject && !existingObject->typesMatch((*object)))
    {
        printWarpError("Setting object of type %s with incompatible type %s",
                       existingObject->typeName().c_str(), object->typeName().c_str());
    }

    // If the creationScope != this then the variable was defined in a parent
    // scope and we are using it. We need to jump up through the scopes and update
    // all scopes using this variable with its new value.
    const Scope *creationScope = objectCreationScope[name];

    if (creationScope != this)
    {
        const_cast<Scope *>(parent)->updateObject(name, object); // Update object in parent scope as well.
    }

    // TODO: - If the existing object being replaced is also in our owned objects set
    // then we need to delete it.
    if (ownedObjects.count(existingObject))
    {
        ownedObjects.erase(existingObject);
        delete existingObject;
    }

    // Update the mapping in our scope.
    objects[name] = object;
}


void Scope::checkForVariableNameClash(const std::string &name) const
{
    if (objectCreatedInScope(name))
    {
        printWarpError("Multiple variables defined in scope with the same name '%s'.", name.c_str());
    }
}


void Scope::removeObject(const std::string &name)
{
    auto existingObject = objects[name];

    // TODO: - should we remove object we own linked to a variable?
    if (existingObject && ownedObjects.count(existingObject))
    {
        delete existingObject;
        ownedObjects.erase(existingObject);
    }

    objects.erase(name);
    objectCreationScope.erase(name);
}
