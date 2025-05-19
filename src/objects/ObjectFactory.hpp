/**
 * @file ObjectFactory.hpp
 * @author Edward Palmer
 * @date 2025-05-16
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include "AnyObject.hpp"
#include <memory>
#include <new>

namespace ObjectFactory
{

template <class... Args>
[[nodiscard]] inline AnyObject::Ptr allocate(Args &&...args)
{
    return std::make_shared<AnyObject>(std::forward<Args>(args)...);
}

AnyObject::Ptr allocate(AnyObject::Type objectType);

} // namespace ObjectFactory