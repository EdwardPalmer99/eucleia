/**
 * @file ClassNode.cpp
 * @author Edward Palmer
 * @date 2024-12-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "ClassNode.hpp"
#include "ClassDefinitionObject.hpp"
#include "ClassObject.hpp"

BaseObject *ClassNode::evaluate(Scope &scope)
{
    ClassDefinitionObject *tempalateObj = scope.getNamedObject<ClassDefinitionObject>(typeName);

    ClassObject *classObj = scope.createManagedObject<ClassObject>(tempalateObj);
    scope.linkObject(name, classObj);

    return classObj;
}