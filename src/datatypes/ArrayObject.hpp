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
#include <cassert>
#include <vector>

class ArrayObject : public BaseObject
{
public:
    ArrayObject() = default;

    ~ArrayObject() override;

    ArrayObject(std::vector<BaseObject *> values_);

    /// Add two array objects and return an unmanaged pointer to result.
    ArrayObject *operator+(const BaseObject &other) const override;

    ArrayObject &operator=(const BaseObject &other) override;

    /// Performs a deep copy of array. This will enable the array to be returned
    /// by a function without objects (defined in function scope) being destroyed.
    ArrayObject *clone() const override;

    BaseObject *operator[](std::size_t index) const
    {
        assert(index < values.size());
        return (values.at(index)); // TODO: - fix this. Just return a reference or something.
    }

    std::vector<BaseObject *> values;
};
