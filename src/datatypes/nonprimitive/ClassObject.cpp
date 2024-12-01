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

    for (FunctionNode *funcNode : classDefinition_->methodDefs)
    {
        // Install in this instance's scope.
        (void)funcNode->evaluate(instanceScope);

        // NB: for now, we do not support method overriding. When performing an
        // assignment, we will only be copying variable values which is already
        // handled by parent class.
    }
}


ClassObject::~ClassObject()
{

}