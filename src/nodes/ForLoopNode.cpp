/**
 * @file ForLoopNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "ForLoopNode.hpp"
#include "ExpressionScope.hpp"
#include "IntObject.hpp"
#include "JumpPoints.hpp"

BaseObject *ForLoopNode::evaluate(Scope &scope)
{
    // Initialization.
    Scope loopScope(scope); // Extend scope.

    start->evaluate(loopScope);

    jmp_buf local;
    pushBreakJumpPoint(&local);

    // Add evaluation to forScope:
    if (setjmp(local) != 1)
    {
        for (;
             evaluate<BoolObject>(condition, loopScope).value; // TODO: - not very efficient repeatedly recalculating...
             update->evaluate(loopScope))
        {
            (void)body->evaluate(loopScope);
        }
    }

    popBreakJumpPoint();

    return nullptr;
}