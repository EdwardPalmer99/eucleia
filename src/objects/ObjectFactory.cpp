/**
 * @file ObjectFactory.cpp
 * @author Edward Palmer
 * @date 2025-05-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "ObjectFactory.hpp"
#include "Exceptions.hpp"
#include <string>

namespace ObjectFactory
{

AnyObject createEmptyObject(AnyObject::Type objectType)
{
    switch (objectType)
    {
        case AnyObject::Int:
            return AnyObject(0L);
        case AnyObject::Bool:
            return AnyObject(false);
        case AnyObject::Float:
            return AnyObject((double)0.0);
        case AnyObject::String:
            return AnyObject(std::string());
        case AnyObject::Array:
            return AnyObject(AnyObject::Vector());
        default:
            ThrowException("cannot create empty AnyObject with type [" + AnyObject::typeToString(objectType) + "]");
    }
}

} // namespace ObjectFactory