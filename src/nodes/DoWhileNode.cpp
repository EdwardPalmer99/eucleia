/**
 * @file DoWhileNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "DoWhileNode.hpp"
#include "EucleiaUtility.hpp"
#include "ExpressionScope.hpp"
#include "IntObject.hpp"
#include "JumpPoints.hpp"

BaseObject *DoWhileNode::evaluate(Scope &scope)
{
    jmp_buf local;
    pushBreakJumpPoint(&local);

    if (setjmp(local) != 1)
    {
        Scope loopScope(scope); // Extend scope.

        do
        {
            (void)body->evaluate(loopScope);
        } while (evaluate<BoolObject>(condition, scope).value);
    }

    // Restore original context.
    popBreakJumpPoint();

    return nullptr; // Return nothing.
}
