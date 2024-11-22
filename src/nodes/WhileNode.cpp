/**
 * @file WhileNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "WhileNode.hpp"
#include "ExpressionScope.hpp"
#include "JumpPoints.hpp"

BaseObject *WhileNode::evaluate(Scope &scope)
{
    // Set jump point for break statements.
    jmp_buf *original = gEnvironmentContext.breakJumpPoint;

    jmp_buf local;
    gEnvironmentContext.breakJumpPoint = &local;

    if (setjmp(local) != 1)
    {
        Scope loopScope(scope); // Extend scope.

        while (evaluate<BoolObject>(condition, scope).value)
        {
            (void)body->evaluate(loopScope);
        }
    }

    // Restore original context.
    gEnvironmentContext.breakJumpPoint = original;

    return nullptr;
}