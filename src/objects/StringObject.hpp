/**
 * @file StringObject.hpp
 * @author Edward Palmer
 * @date 2024-11-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseObjectT.hpp"
#include "IntObject.hpp"
#include <string>

class StringObject : public BaseObjectT<std::string>
{
public:
    StringObject(std::string value = "") : BaseObjectT<std::string>(value) {}

    StringObject &operator=(const BaseObject &other) override
    {
        if (this != &other)
        {
            _value = StringObject::value(other);
        }

        return (*this);
    }

    StringObject operator+(const StringObject &other) const
    {
        return StringObject(_value + other.value());
    }

    StringObject &operator+=(const StringObject &other)
    {
        _value += other.value();
        return *this;
    }


    IntObject operator==(const StringObject &other) const
    {
        return IntObject((_value == other.value()));
    }


    IntObject operator!=(const StringObject &other) const
    {
        return IntObject((_value != other.value()));
    }
};