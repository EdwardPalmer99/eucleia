/**
 * @file LookupVariableNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "LookupVariableNode.hpp"

BaseObject *LookupVariableNode::evaluate(Scope &scope)
{
    return scope.getDefinedObject(variableName);
}