/**
 * @file ClassDefinitionObject.cpp
 * @author Edward Palmer
 * @date 2024-11-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "ClassDefinitionObject.hpp"

ClassDefinitionObject::~ClassDefinitionObject()
{
    // We are responsible for deleting nodes.
    for (auto *ptr : methodDefs)
    {
        delete ptr;
    }

    methodDefs.clear();
}