/**
 * @file StructObject.hpp
 * @author Edward Palmer
 * @date 2024-11-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseObject.hpp"
#include "EucleiaUtility.hpp"
#include "StructDefinitionObject.hpp"
#include <map>
#include <string>

/**
 * struct SomeStruct a;
 */
class StructObject : public BaseObject
{
public:
    StructObject() = delete;
    explicit StructObject(StructDefinitionObject *structDefinition);

    ~StructObject() override;

    StructObject *clone() const override
    {
        EucleiaError("%s", "not implemented!\n");
    }

    std::map<std::string, BaseObject *> objectForName;
};
