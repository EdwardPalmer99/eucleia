/**
 * @file IntObject.hpp
 * @author Edward Palmer
 * @date 2024-11-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseObjectT.hpp"
#include "PoolAllocator.hpp"
#include <cassert>

// 10 chunks per block.
static PoolAllocator allocator{10};

class FloatObject;

/**
 * @brief Long integer type.
 */
class IntObject : public BaseObjectT<long>
{
public:
    IntObject(long value = 0) : BaseObjectT<long>(value) {}

    IntObject &operator=(const BaseObject &other) override
    {
        if (this != &other)
        {
            _value = other.castObject<IntObject>().value();
        }

        return (*this);
    }

    void *operator new(size_t size)
    {
        return allocator.allocate(size);
    }

    void operator delete(void *ptr)
    {
        allocator.deallocate(ptr);
    }

    IntObject *clone() const override
    {
        return new IntObject(_value);
    }

    FloatObject castToFloat() const;

    IntObject &operator++()
    {
        ++_value;
        return *this;
    }

    IntObject &operator--()
    {
        --_value;
        return *this;
    }

    IntObject operator-() const // Negation.
    {
        return IntObject(-_value);
    }

    IntObject operator!() const // Not.
    {
        bool state = (_value > 0);
        return IntObject(!state);
    }

    IntObject operator+(const IntObject &other) const
    {
        return IntObject(_value + other._value);
    }

    IntObject operator-(const IntObject &other) const
    {
        return IntObject(_value - other._value);
    }

    IntObject operator*(const IntObject &other) const
    {
        return IntObject(_value * other._value);
    }

    IntObject operator/(const IntObject &other) const
    {
        return IntObject(_value / other._value);
    }

    IntObject operator==(const IntObject &other) const
    {
        return IntObject(_value == other._value);
    }

    IntObject operator!=(const IntObject &other) const
    {
        return IntObject(_value != other._value);
    }

    IntObject operator>=(const IntObject &other) const
    {
        return IntObject(_value >= other._value);
    }

    IntObject operator>(const IntObject &other) const
    {
        return IntObject(_value > other._value);
    }

    IntObject operator<=(const IntObject &other) const
    {
        return IntObject(_value <= other._value);
    }

    IntObject operator<(const IntObject &other) const
    {
        return IntObject(_value <= other._value);
    }

    IntObject operator%(const IntObject &other) const
    {
        assert(other._value > 0);
        return IntObject(_value % other._value);
    }

    IntObject operator&&(const IntObject &other) const
    {
        return IntObject(_value && other._value);
    }

    IntObject operator||(const IntObject &other) const
    {
        return IntObject(_value || other._value);
    }
};

using BoolObject = IntObject;
