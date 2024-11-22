/**
 * @file AddFloatNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "AddFloatNode.hpp"

FloatObject *AddFloatNode::evaluate(Scope &scope)
{
    return scope.createManagedObject<FloatObject>(floatObject);
}