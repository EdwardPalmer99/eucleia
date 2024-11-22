/**
 * @file Scope.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "Objects.hpp"
#include <map>
#include <memory>
#include <string>
#include <unordered_set>

class Scope
{
public:
    Scope(const Scope &_parent);
    Scope(const Scope *_parent = nullptr);
    ~Scope();

    /// Returns true if object is in current scope.
    bool hasObject(const std::string &name) const;

    /// Get an object from the scope.
    BaseObject *getDefinedObject(const std::string &name) const;

    /// Get an object from the scope and cast to a subclass.
    template <typename T>
    T *getObject(const std::string &name) const
    {
        auto ownedObject = getDefinedObject(name);

        return static_cast<T *>(ownedObject);
    }

    // TODO: - when we create an object with no attached variable, we have to
    // think about whether we should keep it and for how long.

    // TODO: - rename to variable.
    /// Get an object and cast to the desired type. Returns null if not found or
    /// cast fails.
    template <typename T>
    T *getOptionalObject(const std::string &name) const
    {
        if (!hasObject(name))
            return nullptr;

        return static_cast<T *>(getDefinedObject(name));
    }

    /// Create and return a new object in the current scope. We store in the scope
    /// until the destructor is called and then we free memory.
    template <class TObject, class... Args>
    TObject *createManagedObject(Args &&...args)
    {
        TObject *newObject = new TObject(std::forward<Args>(args)...);

        // Assume is derived from BaseObject.
        objectsCreatedInScope.insert(newObject);
        return newObject;
    }

    /// Clones and object and assumes ownership.
    // TODO: - move into cpp file.
    BaseObject *cloneObject(const BaseObject *object)
    {
        if (!object)
            return nullptr;

        BaseObject *newObject = object->clone();
        objectsCreatedInScope.insert(newObject);

        return newObject;
    }

    /// Returns non-const reference to parent scope.
    inline Scope *parentScope() const { return const_cast<Scope *>(parent); }

    // TODO: - rename to variable: defineVariable(name, void *ptr);

    /// Create a link between a variable name and an object owned by the scope.
    void linkObject(const std::string &name, BaseObject *object);

    /// Update the value of an existing object in the current scope.
    void updateLinkedObject(const std::string &name, BaseObject *object);

    /// Returns true if object is in this scope AND object was created in this
    /// scope (as opposed to being created in a parent scope). We cannot have
    /// multiple objects with the same name created in the same scope but we
    /// can have variable shadowing (shadowing variable defined in outer scope).
    bool objectCreatedInScope(const std::string &name) const;

protected:
    void unlinkObject(const std::string &name);

    void checkForVariableNameClash(const std::string &name) const;

private:
    /// Stores a mapping from the variable name to a pointer to the object. These objects
    /// may not belong to the scope (i.e. variables defined in an outer set). Therefore,
    /// when we're doing cleanup, do NOT delete these objects.
    std::map<std::string, BaseObject *> objectForVariableName;

    /// Stores whether a variable was defined in an outer scope. We allow shadow
    /// variables which is when a variable in the current scope hides the definition
    /// of a variable defined in an outer scope.
    std::map<std::string, Scope *> objectCreationScope;

    /// Store all objects we have created. They will persist for the lifetime
    /// of the scope. We are responsible for deleting these in destructor.
    std::unordered_set<BaseObject *> objectsCreatedInScope;

    const Scope *parent{nullptr};
};
