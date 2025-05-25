/**
 * @file Scope.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include <string>
#include <unordered_map>
#include <list>

class Scope
{
public:
    using VariableName = std::string;

    Scope() = default;
    Scope(const Scope &parentScope);

    /// Get a named object ("variable") in our scope or an outer scope. We work outwards from our scope to handle variable shadowing correctly.
    class AnyObject &getObjectRef(const VariableName &name) const;

    class AnyObject *getObjectPtr(const VariableName &name) const;

    /// Create a link between a variable name and an object in this scope.
    class AnyObject &link(const VariableName &name, AnyObject &&object);

    /// Add a link between an already-defined object in this scope and another name to reference it.
    class AnyObject &alias(const VariableName &nameAlias, const VariableName &name);

    /// Returns non-const reference to parent scope.
    inline Scope *parentScope() { return _enclosingScope; }

    /// Set a new parent scope. Use with care!
    void setParentScope(Scope *parent) { _enclosingScope = parent; }

protected:
    /// Throws if the name is already defined in this scope.
    void checkForNameClashesInCurrentScope(const VariableName &name) const;

private:
    using AnyObjectPtrMap = std::unordered_map<VariableName, class AnyObject *>;

    /* Stores references (mapped to _linkedObjects vector) */
    AnyObjectPtrMap _objectPtrMap;

    /* Stores all objects added to scope */
    std::list<class AnyObject> _objects;

    Scope *_enclosingScope{nullptr};
};
