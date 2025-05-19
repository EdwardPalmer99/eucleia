/**
 * @file PropertyInterface.hpp
 * @author Edward Palmer
 * @date 2025-05-12
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "Scope.hpp"

/**
 *
 * 1. RHS of expression: return copy of object (getter).
 * LHS = item.get(0);
 * In this case, we return a copy of the object (we don't want a reference).
 *
 * 2. LHS of expression: set value with RHS (setter).
 * item.set(0, RHS)
 *
 */
struct PropertyInterface
{
    /* Return copy of object (getter) */
    // virtual BaseObject *evaluate(Scope &scope) = 0;

    /* Return object directly for modifying value (setter) */
    virtual std::shared_ptr<class AnyObject> evaluateNoClone(Scope &scope) = 0;

    virtual ~PropertyInterface() = default;
};