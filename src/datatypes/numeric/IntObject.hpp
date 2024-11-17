/**
 * @file IntObject.hpp
 * @author Edward Palmer
 * @date 2024-11-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseObject.hpp"

class FloatObject;

/**
 * @brief Long integer type.
 */
class IntObject : public BaseObject
{
public:
    IntObject(long value_ = 0) : value(value_) {}

    std::string typeName() const override { return "IntObject"; }

    IntObject *clone() const override
    {
        return new IntObject(value);
    }

    FloatObject castToFloat() const;

    IntObject &operator++()
    {
        ++value;
        return *this;
    }

    IntObject &operator--()
    {
        --value;
        return *this;
    }

    IntObject operator-() const // Negation.
    {
        return IntObject(-value);
    }

    IntObject operator!() const // Not.
    {
        bool state = (value > 0);
        return IntObject(!state);
    }

    IntObject operator+(const IntObject &other) const
    {
        return IntObject(value + other.value);
    }

    IntObject operator-(const IntObject &other) const
    {
        return IntObject(value - other.value);
    }

    IntObject operator*(const IntObject &other) const
    {
        return IntObject(value * other.value);
    }

    IntObject operator/(const IntObject &other) const
    {
        return IntObject(value / other.value);
    }

    IntObject operator==(const IntObject &other) const
    {
        return IntObject(value == other.value);
    }

    IntObject operator!=(const IntObject &other) const
    {
        return IntObject(value != other.value);
    }

    IntObject operator>=(const IntObject &other) const
    {
        return IntObject(value >= other.value);
    }

    IntObject operator>(const IntObject &other) const
    {
        return IntObject(value > other.value);
    }

    IntObject operator<=(const IntObject &other) const
    {
        return IntObject(value <= other.value);
    }

    IntObject operator<(const IntObject &other) const
    {
        return IntObject(value <= other.value);
    }

    IntObject operator%(const IntObject &other) const
    {
        assert(other.value > 0);
        return IntObject(value % other.value);
    }

    IntObject operator&&(const IntObject &other) const
    {
        return IntObject(value && other.value);
    }

    IntObject operator||(const IntObject &other) const
    {
        return IntObject(value || other.value);
    }

    long value;
};

using BoolObject = IntObject;