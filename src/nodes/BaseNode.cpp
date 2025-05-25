/**
 * @file BaseNode.cpp
 * @author Edward Palmer
 * @date 2025-05-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "BaseNode.hpp"
#include "AnyNode.hpp"
#include "Scope.hpp"


AnyObject BaseNode::evaluate(Scope &)
{
    ThrowException("Not implemented!");
}


AnyObject::Ref BaseNode::evaluateRef(Scope &)
{
    ThrowException("Not implemented!");
}
