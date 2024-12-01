/**
 * @file StructDefinitionObject.cpp
 * @author Edward Palmer
 * @date 2024-11-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "StructDefinitionObject.hpp"

StructDefinitionObject::~StructDefinitionObject()
{
    // We are responsible for deleting nodes.
    for (auto *ptr : variableDefs)
    {
        delete ptr;
    }

    variableDefs.clear();
}