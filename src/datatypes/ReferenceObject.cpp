/**
 * @file ReferenceObject.cpp
 * @author Edward Palmer
 * @date 2025-01-01
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "ReferenceObject.hpp"

BaseObject *ReferenceObject::clone() const
{
    if (!linkedObject)
    {
        EucleiaError("No linked object. Cannot clone referenced object!");
    }

    return linkedObject->clone(); // Call linked object's clone method.
}