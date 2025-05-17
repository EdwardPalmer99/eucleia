/**
 * @file ObjectFactory.cpp
 * @author Edward Palmer
 * @date 2025-05-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "ObjectFactory.hpp"
#include "ArrayObject.hpp"
#include "Exceptions.hpp"
#include "FloatObject.hpp"
#include "IntObject.hpp"
#include "StringObject.hpp"

namespace ObjectFactory
{

BaseObject::Ptr allocate(ObjectType objectType)
{
    switch (objectType)
    {
        case ObjectType::Int:
        case ObjectType::Bool:
            return allocate<IntObject>();
        case ObjectType::Float:
            return allocate<FloatObject>();
        case ObjectType::String:
            return allocate<StringObject>();
        case ObjectType::Array:
            return allocate<ArrayObject>();
        default:
            ThrowException("cannot allocate for object type!");
    }
}

} // namespace ObjectFactory