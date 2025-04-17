/**
 * @file BoolExpressionScope.hpp
 * @author Edward Palmer
 * @date 2024-11-18
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseNode.hpp"
#include "IntObject.hpp"
#include "Scope.hpp"

/**
 * The goal of this is to be a lightweight wrapper around a node which evaluates
 * to a bool value. This ensures that the objects created in this scope are
 * destroyed once we have a value.
 */
template <typename TValue>
TValue evaluateExpression(BaseNode *node, const Scope &parent)
{
    Scope evalScope(parent);
    TValue output = node->evaluateObject<TValue>(evalScope);

    /* Returns copy */
    return output;
}