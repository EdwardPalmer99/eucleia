/**
 * @file Scope.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseObject.hpp"
#include <new>
#include <string>
#include <unordered_map>
#include <vector>

class Scope
{
public:
    Scope(const Scope &_parent);
    Scope(const Scope *_parent = nullptr);
    ~Scope() = default;

    /// Returns true if named object ("variable") is defined in our scope or in
    /// a parent scope.
    bool hasNamedObject(const std::string &name) const;

    /// Get a named object ("variable") in our scope or an outer scope. We work
    /// outwards from our scope to handle variable shadowing correctly. If the
    /// object is not found, return nullptr.
    BaseObject::Ptr getOptionalNamedObject(const std::string &name) const;

    /// Similar to getOptionalObject but has a check to ensure pointer is valid.
    BaseObject::Ptr getNamedObject(const std::string &name) const;

    /// Get an object from the scope and cast to a subclass.
    template <typename TObject>
    std::shared_ptr<TObject> getNamedObject(const std::string &name) const
    {
        auto objectPtr = getNamedObject(name);
        return std::static_pointer_cast<TObject>(objectPtr);
    }

    template <typename TObject>
    std::shared_ptr<TObject> getOptionalNamedObject(const std::string &name) const
    {
        BaseObject::Ptr obj = getOptionalNamedObject(name);
        if (!obj)
        {
            return nullptr;
        }

        return std::static_pointer_cast<TObject>(obj);
    }

    /// Returns non-const reference to parent scope.
    inline Scope *parentScope() { return parent; }

    /// Set a new parent scope. Use with care!
    void setParentScope(Scope *parent_) { parent = parent_; }

    /// Create a link between a variable name and an object in this scope.
    void linkObject(const std::string &name, BaseObject::Ptr object);

private:
    /// Stores a mapping from the variable name to a pointer to the object. These
    /// are only linked objects defined in this scope. This enables variable
    /// shadowing.
    std::unordered_map<std::string, BaseObject::Ptr> linkedObjectForName;

    Scope *parent{nullptr};
};
