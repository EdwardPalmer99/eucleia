/**
 * @file ArrayAccessNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "ArrayAccessNode.hpp"
#include "ArrayObject.hpp"
#include "BaseNode.hpp"
#include <cassert>
#include <iostream>

ArrayAccessNode::~ArrayAccessNode()
{
    delete arrayLookup;
    delete index;
}


BaseObject *ArrayAccessNode::evaluateNoClone(Scope &scope)
{
    // Lookup in array.
    auto &arrayObj = arrayLookup->evaluate(scope)->castObject<ArrayObject>();
    auto &indexObject = index->evaluate(scope)->castObject<IntObject>();

    return arrayObj[indexObject.value];
}


BaseObject *ArrayAccessNode::evaluate(Scope &scope)
{
    BaseObject *currentObject = evaluateNoClone(scope);

    // Return a safe copy of object.
    return scope.cloneObject(currentObject);
}
