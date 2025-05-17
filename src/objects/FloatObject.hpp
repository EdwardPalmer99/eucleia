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
#include "ObjectFactory.hpp"
#include <memory>

/**
 * Class for double type.
 */
class FloatObject : public BaseObjectT<double>
{
public:
    using Ptr = std::shared_ptr<FloatObject>;

    FloatObject(double value = 0.0) : BaseObjectT<double>(value) {}

    FloatObject &operator=(const BaseObject &other) override
    {
        if (this != &other)
        {
            _value = FloatObject::value(other);
        }

        return (*this);
    }

    BaseObject::Ptr clone() const override
    {
        return ObjectFactory::allocate<FloatObject>(_value);
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
