/**
 * @file ObjectTypes.cpp
 * @author Edward Palmer
 * @date 2025-01-01
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "ObjectTypes.hpp"
#include "Exceptions.hpp"
#include <unordered_map>

ObjectType objectTypeForName(const std::string &typeName)
{
    // NB: mark as static to persist.
    static std::unordered_map<std::string, ObjectType> objectTypeMap{
        {"int", ObjectType::Int},
        {"bool", ObjectType::Bool},
        {"float", ObjectType::Float},
        {"string", ObjectType::String},
        {"array", ObjectType::Array},
        {"struct", ObjectType::Struct},
        {"class", ObjectType::Class}};

    auto iter = objectTypeMap.find(typeName);
    if (iter != objectTypeMap.end())
    {
        return iter->second;
    }

    // Uh-oh. Unrecognized type-name.
    ThrowException("unrecognized type-name: " + typeName);
}