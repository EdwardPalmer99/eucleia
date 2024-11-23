/**
 * @file FloatObject.cpp
 * @author Edward Palmer
 * @date 2024-11-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "FloatObject.hpp"

FloatObject IntObject::castToFloat() const
{
    return FloatObject((double)value);
}
