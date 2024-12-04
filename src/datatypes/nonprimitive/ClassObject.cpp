/**
 * @file ClassObject.cpp
 * @author Edward Palmer
 * @date 2024-11-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "ClassObject.hpp"
#include "FunctionNode.hpp"
#include <cassert>

ClassObject::ClassObject(ClassDefinitionObject *classDefinition_)
    : StructObject(classDefinition_)
{
    assert(classDefinition_);

    for (auto &[name, funcNode] : classDefinition_->allMethodDefsMap)
    {
        // Install in this instance's scope.
        (void)funcNode->evaluate(instanceScope);
    }
}


ClassObject::~ClassObject()
{
}