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

ArrayAccessNode::~ArrayAccessNode()
{
    delete arrayLookup;
    delete index;
}

BaseObject *ArrayAccessNode::evaluate(Scope &scope)
{
    // Lookup array in scope or parent scope.
    ArrayObject *arrayObject = static_cast<ArrayObject *>(arrayLookup->evaluate(scope));

    Scope arrayAccessScope(scope); // TODO: - inefficient. Should not store Rvalues in scope.
    IntObject *indexObject = index->evaluate(arrayAccessScope);

    BaseObject *lookupObj = arrayObject->values[indexObject->value];

    // Now return a copy of lookupObj.
    return lookupObj->clone();
}