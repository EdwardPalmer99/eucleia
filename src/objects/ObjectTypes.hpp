/**
 * @file ObjectTypes.hpp
 * @author Edward Palmer
 * @date 2024-11-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include <string>

/**
 * enum for supported object types.
 */
enum class ObjectType
{
    Int,
    Bool,
    Float,
    String,
    Array,
    Struct,
    Class
};

/**
 * Returns an object type for the type name.
 */
ObjectType objectTypeForName(const std::string &typeName);