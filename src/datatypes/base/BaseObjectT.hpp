/**
 * @file BaseObjectT.hpp
 * @author Edward Palmer
 * @date 2025-04-13
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "BaseObject.hpp"

/* Template wrapper around BaseObject with accessors */
template <class TValue>
class BaseObjectT : public BaseObject
{
public:
    /* Constructor */
    explicit BaseObjectT(TValue value) : _value(std::move(value)) {}

    /* Returns non-const accessor */
    [[nodiscard]] TValue &operator*()
    {
        return _value;
    }

    /* Returns const accessor */
    [[nodiscard]] const TValue &operator*() const
    {
        return _value;
    }

    /* Overrides clone method */
    BaseObjectT<TValue> *clone() const override
    {
        return new BaseObjectT<TValue>(_value);
    }

    /* Const-accessor */
    [[nodiscard]] const TValue &value() const
    {
        return _value;
    }

    /* Non-const accessor */
    [[nodiscard]] TValue &value()
    {
        return _value;
    }

protected:
    /* Enable subclasses to use default constructor */
    BaseObjectT() = default;

    /* Stores some value */
    TValue _value;
};
