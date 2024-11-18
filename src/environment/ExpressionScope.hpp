/**
 * @file BoolExpressionScope.hpp
 * @author Edward Palmer
 * @date 2024-11-18
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "EucleiaNode.hpp"
#include "EucleiaScope.hpp"
#include "IntObject.hpp"

/**
 * The goal of this is to be a lightweight wrapper around a node which evaluates
 * to a bool value. This ensures that the objects created in this scope are
 * destroyed once we have a value.
 */
template <class TObject>
static TObject evaluate(BaseNode *node, const Scope &parent)
{
    Scope evalScope(parent);
    auto *output = node->evaluate<TObject>(evalScope);

    return (*output); // Copy.
}