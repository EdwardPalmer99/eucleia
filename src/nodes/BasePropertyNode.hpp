/**
 * @file BasePropertyNode.hpp
 * @author Edward Palmer
 * @date 2024-11-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseNode.hpp"
#include "Scope.hpp"

/**
 *
 * 1. RHS of expression: return copy of object (getter).
 * LHS = item.get(0);
 * In this case, we return a copy of the object (we don't want a reference).
 *
 * 2. LHS of expression: set value with RHS (setter).
 * item.set(0, RHS)
 */
class BasePropertyNode : public BaseNode // TODO: - should be interface.
{
public:
    // Return copy of object (getter).
    BaseObject *evaluate(Scope &scope) override = 0;

    // Return object directly (not copy!). Should be used to modify value.
    virtual BaseObject *evaluateNoClone(Scope &scope) = 0;
};