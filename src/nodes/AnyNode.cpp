/**
 * @file AnyNode.cpp
 * @author Edward Palmer
 * @date 2025-05-19
 *
 * @copyright Copyright (c) 2025
 *
 */


#include "AnyNode.hpp"
#include "AnyObject.hpp"

std::shared_ptr<AnyObject> AnyNode::evaluate(Scope &scope)
{
    return _evaluateFunc(scope);
}