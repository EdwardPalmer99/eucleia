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
#include <vector>

class ArrayObject : public BaseObject
{
public:
    ArrayObject() = default;
    ArrayObject(std::vector<BaseObject *> values_) : values(std::move(values_)) {}

    // TODO: - who owns the objects in the array? MEMORY LEAK!!
    /// Performs a deep copy of array. This will enable the array to be returned
    /// by a function without objects (defined in function scope) being destroyed.
    ArrayObject *clone() const override
    {
        std::vector<BaseObject *> cloneValues(values.size());

        for (BaseObject *obj : values)
        {
            cloneValues.push_back(obj->clone());
        }

        return new ArrayObject(cloneValues);
    }

    // TODO: - eventually just store references to BaseObject & or pointers and return reference.
    BaseObject *operator[](std::size_t index) const
    {
        assert(index < values.size());
        return (values.at(index)); // TODO: - fix this. Just return a reference or something.
    }

    std::vector<BaseObject *> values;
};
