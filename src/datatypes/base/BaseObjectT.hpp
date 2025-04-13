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
template <class TStored>
class BaseObjectT : public BaseObject
{
public:
    /* Returns const accessor */
    [[nodiscard]] const TStored &operator*() const
    {
        return _values;
    }

    /* Returns non-const accessor */
    [[nodiscard]] const TStored &operator*()
    {
        return _values;
    }

protected:
    BaseObjectT() = default;

    /* Accessor for child classes */
    [[nodiscard]] TStored &values()
    {
        return values;
    }

private:
    TStored _values;
};
