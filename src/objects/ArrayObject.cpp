/**
 * @file ArrayObject.cpp
 * @author Edward Palmer
 * @date 2024-11-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "ArrayObject.hpp"
#include "ObjectFactory.hpp"

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
    _value.resize(otherArray.value().size());

    for (size_t i = 0; i < otherArray.value().size(); ++i)
    {
        BaseObject::Ptr otherObj = otherArray[i];

        _value[i] = otherObj->clone();
    }

    return (*this);
}


ArrayObject ArrayObject::operator+(const BaseObject &other) const
{
    const ArrayObject &otherArray = other.castObject<ArrayObject>();

    // NB: special case where we add ourself to ourself. Currently, we will be
    // cloning the same object twice which is okay. But might want to think about
    // references in future.

    // Create an array to store objects from both.
    BaseObjectPtrVector combinedArray;
    combinedArray.reserve(_value.size() + otherArray.value().size());

    // Now we iterate over array and add to vector.
    for (auto &object : _value)
    {
        combinedArray.push_back(object->clone());
    }

    for (auto &object : otherArray.value())
    {
        combinedArray.push_back(object->clone());
    }

    // Now create new array object. Caller responsible for handling memory.
    // Ideally, should be added to scope of caller.
    return ArrayObject(std::move(combinedArray));
}


ArrayObject::Ptr ArrayObject::clone() const
{
    BaseObjectPtrVector cloneValues(value().size());

    for (auto &obj : value())
    {
        cloneValues.push_back(obj->clone());
    }

    return ObjectFactory::allocate<ArrayObject>(std::move(cloneValues));
}