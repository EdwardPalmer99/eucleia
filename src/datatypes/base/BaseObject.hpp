/**
 * @file BaseObject.hpp
 * @author Edward Palmer
 * @date 2024-11-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include <string>

/**
 * BaseObject. All objects are derived from this class.
 */
class BaseObject
{
public:
    virtual ~BaseObject() = default;

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

    virtual std::string typeName() const = 0;

protected:
    BaseObject() = default;
};