/**
 * @file StructDefinitionObject.hpp
 * @author Edward Palmer
 * @date 2024-11-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseObject.hpp"
#include <map>
#include <string>

/**
 * struct SomeStruct
 * {
 *      int a;
 *      float f;
 *      string s;
 * };
 */
class StructDefinitionObject : public BaseObject
{
public:
    enum class SupportedObjectTypes
    {
        IntObject,
        FloatObject,
        StringObject
    };

    StructDefinitionObject(std::map<std::string, SupportedObjectTypes> memberVariableTypes_)
        : memberVariableTypes(std::move(memberVariableTypes_)) {}

    StructDefinitionObject *clone() const override
    {
        return new StructDefinitionObject(memberVariableTypes);
    }

    std::map<std::string, SupportedObjectTypes> memberVariableTypes;
};