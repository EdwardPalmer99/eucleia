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

using ArrayValue = std::vector<BaseObject *>;

class ArrayObject : public BaseObjectT<ArrayValue>
{
public:
    ArrayObject() = default;
    ArrayObject(ArrayValue values);

    ~ArrayObject() override;


    /// Add two array objects and return an unmanaged pointer to result.
    ArrayObject *operator+(const BaseObject &other) const override;

    ArrayObject &operator=(const BaseObject &other) override;

    /// Performs a deep copy of array. This will enable the array to be returned
    /// by a function without objects (defined in function scope) being destroyed.
    ArrayObject *clone() const override;

    BaseObject *operator[](std::size_t index) const
    {
        assert(index < value().size());
        return (value().at(index)); // TODO: - fix this. Just return a reference or something.
    }
};
