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
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

class Scope
{
public:
    Scope(const Scope &_parent);
    Scope(const Scope *_parent = nullptr);
    ~Scope();

    /// Returns true if named object ("variable") is defined in our scope or in
    /// a parent scope.
    bool hasNamedObject(const std::string &name) const;

    /// Get a named object ("variable") in our scope or an outer scope. We work
    /// outwards from our scope to handle variable shadowing correctly. If the
    /// object is not found, return nullptr.
    BaseObject *getOptionalNamedObject(const std::string &name) const;

    /// Similar to getOptionalObject but has a check to ensure pointer is valid.
    BaseObject *getNamedObject(const std::string &name) const;

    /// Get an object from the scope and cast to a subclass.
    template <typename T>
    T *getNamedObject(const std::string &name) const
    {
        BaseObject *ownedObject = getNamedObject(name);
        return static_cast<T *>(ownedObject);
    }

    template <typename T>
    T *getOptionalNamedObject(const std::string &name) const
    {
        BaseObject *obj = getOptionalNamedObject(name);
        if (!obj)
        {
            return nullptr;
        }

        return static_cast<T *>(obj);
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
        {
            return nullptr;
        }

        BaseObject *newObject = object->clone();
        objectsCreatedInScope.insert(newObject);

        return newObject;
    }

    /// Returns non-const reference to parent scope.
    inline Scope *parentScope() const { return const_cast<Scope *>(parent); }

    // TODO: - rename to variable: defineVariable(name, void *ptr);

    /// Create a link between a variable name and an object in this scope.
    void linkObject(const std::string &name, BaseObject *object);

    /// Updates the link between a variable and an object in this scope or the
    /// first outer scope. The object type must match the existing object type.
    void updateLinkedObject(const std::string &name, BaseObject *object);

private:
    /// Stores a mapping from the variable name to a pointer to the object. These
    /// are only linked objects defined in this scope. This enables variable
    /// shadowing.
    std::unordered_map<std::string, BaseObject *> linkedObjectForName;

    /// Store all objects we have created. They will persist for the lifetime
    /// of the scope. We are responsible for deleting these in destructor.
    std::unordered_set<BaseObject *> objectsCreatedInScope;

    const Scope *parent{nullptr};
};
