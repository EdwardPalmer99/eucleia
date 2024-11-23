/**
 * @file AddArrayNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "AddArrayNode.hpp"

ArrayObject *AddArrayNode::evaluate(Scope &scope)
{
    std::vector<BaseObject *> evaluatedObjects;

    // Scope owns all objects in array and array itself.
    for (auto *node : programNodes)
    {
        evaluatedObjects.push_back(node->evaluate(scope));
    }

    evaluatedObjects.shrink_to_fit();

    return scope.createManagedObject<ArrayObject>(evaluatedObjects);
}