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
#include "ObjectFactory.hpp"
#include <utility>

/* Template wrapper around BaseObject with accessors */
template <class TValue>
class BaseObjectT : public BaseObject
{
public:
    /* Use this to ensure that stored value type is correct */
    using Type = TValue;

    virtual BaseObject &operator=(const BaseObject &other) override
    {
        return BaseObject::operator=(other);
    }

    /* Elegant wrapper to extract value from a base object */
    static const TValue &value(const BaseObject &obj)
    {
        const auto &upcast = static_cast<const BaseObjectT<TValue> &>(obj);

        return (*upcast); /* Return value */
    }

    static TValue &value(BaseObject &obj)
    {
        const auto &upcast = static_cast<BaseObjectT<TValue> &>(obj);

        return (*upcast); /* Return value */
    }

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
