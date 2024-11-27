/**
 * @file ArrayObject.cpp
 * @author Edward Palmer
 * @date 2024-11-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "ArrayObject.hpp"

ArrayObject::ArrayObject(std::vector<BaseObject *> values_)
{
    values.reserve(values_.size());

    // Clone each object in vector. We have ownership of these! The objects in
    // the vector probably are owned by some scope and we do not want to share
    // ownership.
    for (BaseObject *obj : values_)
    {
        values.push_back(obj->clone());
    }
}

ArrayObject::~ArrayObject()
{
    // Delete objects we have ownership of.
    for (BaseObject *obj : values)
    {
        delete obj;
    }

    values.clear();
}

ArrayObject &ArrayObject::operator=(const BaseObject &other)
{
    if (this == &other)
    {
        return (*this);
    }

    const ArrayObject &otherArray = other.castObject<ArrayObject>();

    // NB: - when we copy the array, we clone all values inside and have the
    // responsibility of managing their memory. The scope only owns the array
    // and not the objects inside the array!
    // 1. clear objects in existing vector.
    for (BaseObject *obj : values)
    {
        delete obj;
    }

    values.resize(otherArray.values.size());

    for (size_t i = 0; i < otherArray.values.size(); ++i)
    {
        BaseObject *otherObj = otherArray[i];

        values[i] = otherObj->clone();
    }

    return (*this);
}


ArrayObject *ArrayObject::clone() const
{
    std::vector<BaseObject *> cloneValues(values.size());

    for (BaseObject *obj : values)
    {
        cloneValues.push_back(obj->clone());
    }

    return new ArrayObject(cloneValues);
}