/**
 * @file BaseObject.hpp
 * @author Edward Palmer
 * @date 2024-11-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "Exceptions.hpp"
#include <string>
#include <typeinfo>
#include <vector>

/**
 * BaseObject. All objects are derived from this class.
 */
class BaseObject
{
public:
    virtual ~BaseObject() = default;

    // Implement copy assignment in derived classes.
    virtual BaseObject &operator=(const BaseObject &other)
    {
        ThrowException("copy assignment not implemented");
    }

    // Implement addition. Returns unmanaged pointer to result.
    virtual BaseObject *operator+(const BaseObject &other) const
    {
        ThrowException("addition not implemented");
    }

    /// Cast to object type.
    template <class TObject>
    const TObject &castObject() const
    {
        return static_cast<const TObject &>(*this);
    }

    template <class TObject>
    TObject &castObject()
    {
        return static_cast<TObject &>(*this);
    }

    /// Check object type.
    template <class TObject>
    bool isObjectType() const
    {
        return typeid(*this) == typeid(TObject);
    }

    bool typesMatch(const BaseObject &other) const
    {
        return typeid(*this) == typeid(other);
    }

    // TODO: - clone should be called with a scope specified to handle memory.
    /// Implement creating a copy for derived classes.
    virtual BaseObject *clone() const = 0;

protected:
    BaseObject() = default;
};

using BaseObjectPtrVector = std::vector<BaseObject *>;
