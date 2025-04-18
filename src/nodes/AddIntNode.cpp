/**
 * @file AddIntNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "AddIntNode.hpp"
#include <memory>

IntObject *AddIntNode::evaluate(Scope &scope)
{
    return scope.createManagedObject<IntObject>(intObject);
}
