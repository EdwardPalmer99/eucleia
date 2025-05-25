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

AnyObject createEmptyObject(AnyObject::Type objectType);

} // namespace ObjectFactory