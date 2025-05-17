/**
 * @file LookupVariableNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "LookupVariableNode.hpp"


BaseObject::Ptr LookupVariableNode::evaluate(Scope &scope)
{
    return scope.getNamedObject(_name);
}