/**
 * @file NodeFactory.cpp
 * @author Edward Palmer
 * @date 2025-05-05
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "NodeFactory.hpp"
#include "BaseObject.hpp"
#include "ExpressionScope.hpp"
#include "JumpPoints.hpp"
#include "Objects.hpp"


namespace NodeFactory
{

namespace Loops
{
AnyNode createWhileNode(AnyNode condition, AnyNode body)
{
    return AnyNode([condition = std::move(condition), body = std::move(body)](Scope &scope) -> BaseObject *
    {
        // Set jump point for break statements.
        jmp_buf local;
        pushBreakJumpPoint(&local);

        if (setjmp(local) != 1)
        {
            Scope loopScope(scope); // Extend scope.

            while (evaluateExpression<BoolObject::Type>(condition, scope))
            {
                (void)body(loopScope);
            }
        }

        // Restore original context.
        popBreakJumpPoint();

        return nullptr;
    });
}


AnyNode createDoWhileNode(AnyNode condition, AnyNode body)
{
    return AnyNode([condition = std::move(condition), body = std::move(body)](Scope &scope) -> BaseObject *
    {
        jmp_buf local;
        pushBreakJumpPoint(&local);

        if (setjmp(local) != 1)
        {
            Scope loopScope(scope); // Extend scope.

            do
            {
                (void)body(loopScope);
            } while (evaluateExpression<BoolObject::Type>(condition, scope));
        }

        // Restore original context.
        popBreakJumpPoint();

        return nullptr; // Return nothing.
    });
}


AnyNode createForLoopNode(AnyNode start, AnyNode condition, AnyNode update, AnyNode body)
{
    return AnyNode([start = std::move(start), condition = std::move(condition), update = std::move(update), body = std::move(body)](Scope &scope) -> BaseObject *
    {
        // Initialization.
        Scope loopScope(scope); // Extend scope.

        (void)start(loopScope);

        jmp_buf local;
        pushBreakJumpPoint(&local);

        // Add evaluation to forScope:
        if (setjmp(local) != 1)
        {
            for (;
                 evaluateExpression<BoolObject::Type>(condition, loopScope); // TODO: - not very efficient repeatedly recalculating...
                 update(loopScope))
            {
                (void)body(loopScope);
            }
        }

        popBreakJumpPoint();

        return nullptr;
    });
}
} // namespace Loops

} // namespace NodeFactory