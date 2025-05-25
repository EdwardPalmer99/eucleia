/**
 * @file AnyNode.cpp
 * @author Edward Palmer
 * @date 2025-05-19
 *
 * @copyright Copyright (c) 2025
 *
 */


#include "AnyNode.hpp"


AnyObject AnyNode::evaluate(Scope &scope)
{
    return _evaluateFunc(scope);
}


AnyObject &AnyPropertyNode::evaluateRef(Scope &scope)
{
    return _evaluateRefFunc(scope);
}