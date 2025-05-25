/**
 * @file LookupVariableNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "LookupVariableNode.hpp"
#include "AnyObject.hpp"

AnyObject::Ref LookupVariableNode::evaluateRef(Scope &scope)
{
    return scope.getObjectRef(_name);
}


AnyObject LookupVariableNode::evaluate(Scope &scope)
{
    return evaluateRef(scope); /* Copy object. Avoid if possible */
}