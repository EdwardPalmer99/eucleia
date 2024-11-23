/**
 * @file StringObject.hpp
 * @author Edward Palmer
 * @date 2024-11-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseObject.hpp"
#include "IntObject.hpp"
#include <string>

class StringObject : public BaseObject
{
public:
    StringObject(std::string value_ = "") : value(std::move(value_)) {}

    StringObject *clone() const override
    {
        return new StringObject(value);
    }

    StringObject operator+(const StringObject &other) const
    {
        return StringObject(value + other.value);
    }

    StringObject &operator+=(const StringObject &other)
    {
        value += other.value;
        return *this;
    }


    IntObject operator==(const StringObject &other) const
    {
        return IntObject((value == other.value));
    }


    IntObject operator!=(const StringObject &other) const
    {
        return IntObject((value != other.value));
    }

    std::string value;
};