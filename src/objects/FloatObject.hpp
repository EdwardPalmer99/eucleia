/**
 * @file FloatObject.hpp
 * @author Edward Palmer
 * @date 2024-11-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseObject.hpp"
#include "IntObject.hpp"

/**
 * Class for double type.
 */
class FloatObject : public BaseObject
{
public:
    FloatObject(double value_ = 0.0) : value(value_) {}

    std::string typeName() const override { return "FloatObject"; }

    FloatObject *clone() const override
    {
        return new FloatObject(value);
    }

    FloatObject &operator++()
    {
        ++value;
        return *this;
    }

    FloatObject &operator--()
    {
        --value;
        return *this;
    }

    FloatObject operator-() const // Negation.
    {
        return FloatObject(-value);
    }

    // Add addition / subtraction operations.
    FloatObject operator+(const FloatObject &other) const
    {
        return FloatObject(value + other.value);
    }

    FloatObject operator-(const FloatObject &other) const
    {
        return FloatObject(value - other.value);
    }

    FloatObject operator*(const FloatObject &other) const
    {
        return FloatObject(value * other.value);
    }

    FloatObject operator/(const FloatObject &other) const
    {
        return FloatObject(value / other.value);
    }

    IntObject operator==(const FloatObject &other) const
    {
        return IntObject(value == other.value);
    }

    IntObject operator!=(const FloatObject &other) const
    {
        return IntObject(value != other.value);
    }

    IntObject operator>=(const FloatObject &other) const
    {
        return IntObject(value >= other.value);
    }

    IntObject operator>(const FloatObject &other) const
    {
        return IntObject(value > other.value);
    }

    IntObject operator<=(const FloatObject &other) const
    {
        return IntObject(value <= other.value);
    }

    IntObject operator<(const FloatObject &other) const
    {
        return IntObject(value <= other.value);
    }

    double value;
};
