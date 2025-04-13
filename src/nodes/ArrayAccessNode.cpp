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
#include "BaseObjectT.hpp"

ArrayAccessNode::~ArrayAccessNode()
{
    delete arrayLookup;
    delete index;
}


BaseObject *ArrayAccessNode::evaluateNoClone(Scope &scope)
{
    // Lookup in array.
    auto &arrayObj = arrayLookup->evaluate(scope)->castObject<ArrayObject>();

    return arrayObj[index->evaluateObject<IntObject::Type>(scope)];
}


BaseObject *ArrayAccessNode::evaluate(Scope &scope)
{
    BaseObject *currentObject = evaluateNoClone(scope);

    // Return a safe copy of object.
    return scope.cloneObject(currentObject);
}
