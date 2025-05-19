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

AnyObject::Ptr allocate(AnyObject::Type objectType)
{
    switch (objectType)
    {
        case AnyObject::Int:
            return std::make_shared<AnyObject>(0L);
        case AnyObject::Bool:
            return std::make_shared<AnyObject>(false);
        case AnyObject::Float:
            return std::make_shared<AnyObject>((double)0.0);
        case AnyObject::String:
            return std::make_shared<AnyObject>(std::string());
        case AnyObject::Array:
            return std::make_shared<AnyObject>(AnyObject::Vector());
        default:
            ThrowException("cannot allocate for object type!");
    }
}

} // namespace ObjectFactory