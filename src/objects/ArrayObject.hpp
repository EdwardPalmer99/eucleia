/**
 * @file ArrayObject.hpp
 * @author Edward Palmer
 * @date 2024-11-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseObject.hpp"
#include "BaseObjectT.hpp"
#include <cassert>
#include <vector>

class ArrayObject : public BaseObjectT<BaseObjectPtrVector>
{
public:
    ArrayObject() = default;
    ~ArrayObject() override = default;

    ArrayObject(BaseObjectPtrVector objects) : BaseObjectT<BaseObjectPtrVector>(std::move(objects)) {}

    /// Add two array objects and return an unmanaged pointer to result.
    ArrayObject operator+(const BaseObject &other) const;

    ArrayObject &operator=(const BaseObject &other) override;

    /// Performs a deep copy of array. This will enable the array to be returned
    /// by a function without objects (defined in function scope) being destroyed.
    BaseObject::Ptr clone() const override;

    BaseObject::Ptr operator[](std::size_t index) const
    {
        assert(index < value().size());
        return (value().at(index));
    }
};
