/**
 * @file Scope.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include <memory>
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
    std::shared_ptr<class AnyObject> getOptionalNamedObject(const std::string &name) const;

    /// Similar to getOptionalObject but has a check to ensure pointer is valid.
    std::shared_ptr<class AnyObject> getNamedObject(const std::string &name) const;

    /// Returns non-const reference to parent scope.
    inline Scope *parentScope() { return parent; }

    /// Set a new parent scope. Use with care!
    void setParentScope(Scope *parent_) { parent = parent_; }

    /// Create a link between a variable name and an object in this scope.
    void linkObject(const std::string &name, std::shared_ptr<class AnyObject> object);

private:
    /// Stores a mapping from the variable name to a pointer to the object. These
    /// are only linked objects defined in this scope. This enables variable
    /// shadowing.
    std::unordered_map<std::string, std::shared_ptr<class AnyObject>> linkedObjectForName;

    Scope *parent{nullptr};
};
