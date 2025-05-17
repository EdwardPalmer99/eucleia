/**
 * @file FunctionNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "FunctionNode.hpp"
#include "FunctionObject.hpp"
#include "ObjectFactory.hpp"

/// Create a new FunctionObject from a FunctionNode and register in current scope.
BaseObject::Ptr FunctionNode::evaluate(Scope &scope)
{
    // TODO: - I think this creates a strong-reference cycle! Need to break the chain here
    auto functionObject = ObjectFactory::allocate<FunctionObject>(shared_from_this());

    scope.linkObject(_funcName, functionObject);

    return functionObject;
}