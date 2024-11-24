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
#include <map>
#include <string>

/**
 * struct SomeStruct
 * {
 *      int a;
 *      string b;
 *      ...
 * };
 */
class StructObject : public BaseObject
{
public:
    StructObject(std::map<std::string, BaseObject *> items_) : objectForName(std::move(items_)) {}

    StructObject *clone() const override
    {
        printEucleiaError("%s", "not implemented!\n");
    }

protected:
    std::map<std::string, BaseObject *> objectForName;
};
