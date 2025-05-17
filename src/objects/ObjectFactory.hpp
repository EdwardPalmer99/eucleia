/**
 * @file ObjectFactory.hpp
 * @author Edward Palmer
 * @date 2025-05-16
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include "BaseObject.hpp"
#include "ObjectTypes.hpp"
#include <memory>
#include <new>

namespace ObjectFactory
{

template <class TObject, class... Args>
[[nodiscard]] inline std::shared_ptr<TObject> allocate(Args &&...args)
{
    return std::make_shared<TObject>(std::forward<Args>(args)...);
}

BaseObject::Ptr allocate(ObjectType objectType);

} // namespace ObjectFactory