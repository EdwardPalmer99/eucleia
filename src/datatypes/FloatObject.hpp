/**
 * @file FloatObject.hpp
 * @author Edward Palmer
 * @date 2024-11-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseObjectT.hpp"
#include "IntObject.hpp"

/**
 * Class for double type.
 */
class FloatObject : public BaseObjectT<double>
{
public:
    FloatObject(double value = 0.0) : BaseObjectT<double>(value) {}

    FloatObject &operator=(const BaseObject &other) override
    {
        if (this != &other)
        {
            _value = FloatObject::value(other);
        }

        return (*this);
    }

    FloatObject *clone() const override
    {
        return new FloatObject(_value);
    }

    FloatObject &operator++()
    {
        ++_value;
        return *this;
    }

    FloatObject &operator--()
    {
        --_value;
        return *this;
    }

    FloatObject operator-() const // Negation.
    {
        return FloatObject(-_value);
    }

    // Add addition / subtraction operations.
    FloatObject operator+(const FloatObject &other) const
    {
        return FloatObject(_value + other._value);
    }

    FloatObject operator-(const FloatObject &other) const
    {
        return FloatObject(_value - other._value);
    }

    FloatObject operator*(const FloatObject &other) const
    {
        return FloatObject(_value * other._value);
    }

    FloatObject operator/(const FloatObject &other) const
    {
        return FloatObject(_value / other._value);
    }

    IntObject operator==(const FloatObject &other) const
    {
        return IntObject(_value == other._value);
    }

    IntObject operator!=(const FloatObject &other) const
    {
        return IntObject(_value != other._value);
    }

    IntObject operator>=(const FloatObject &other) const
    {
        return IntObject(_value >= other._value);
    }

    IntObject operator>(const FloatObject &other) const
    {
        return IntObject(_value > other._value);
    }

    IntObject operator<=(const FloatObject &other) const
    {
        return IntObject(_value <= other._value);
    }

    IntObject operator<(const FloatObject &other) const
    {
        return IntObject(_value < other._value);
    }
};
