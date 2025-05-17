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
#include <memory>
#include <string>
#include <typeinfo>
#include <vector>

/**
 * BaseObject. All objects are derived from this class.
 */
class BaseObject
{
public:
    using Ptr = std::shared_ptr<BaseObject>;

    virtual ~BaseObject() = default;

    // Implement copy assignment in derived classes.
    virtual BaseObject &operator=(const BaseObject &)
    {
        ThrowException("copy assignment not implemented");
    }

    /// Cast to object type.
    template <class TObject>
    [[nodiscard]] const TObject &castObject() const
    {
        return static_cast<const TObject &>(*this);
    }

    template <class TObject>
    [[nodiscard]] TObject &castObject()
    {
        return static_cast<TObject &>(*this);
    }

    /// Check object type.
    template <class TObject>
    [[nodiscard]] bool isObjectType() const
    {
        return typeid(*this) == typeid(TObject);
    }

    [[nodiscard]] virtual BaseObject::Ptr clone() const
    {
        ThrowException("clone is not implemented");
    }

protected:
    BaseObject() = default;
};

using BaseObjectPtrVector = std::vector<BaseObject::Ptr>;