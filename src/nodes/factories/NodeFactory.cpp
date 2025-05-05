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
#include "IntObject.hpp"
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


namespace Instances
{
AnyNode createBoolNode(bool state)
{
    return AnyNode([state](Scope &scope) -> BaseObject *
    {
        return scope.createManagedObject<BoolObject>(state);
    });
}

AnyNode createIntNode(long value)
{
    return AnyNode([value](Scope &scope) -> BaseObject *
    {
        return scope.createManagedObject<IntObject>(value);
    });
}

AnyNode createFloatNode(double value)
{
    return AnyNode([value](Scope &scope) -> BaseObject *
    {
        return scope.createManagedObject<FloatObject>(value);
    });
}

AnyNode createStringNode(std::string value)
{
    return AnyNode([value = std::move(value)](Scope &scope) -> BaseObject *
    {
        return scope.createManagedObject<StringObject>(value);
    });
}

AnyNode createArrayNode(std::vector<AnyNode> elementNodes)
{
    return AnyNode([elementNodes = std::move(elementNodes)](Scope &scope) -> BaseObject *
    {
        /* Evaluate each element node */
        std::vector<BaseObject *> elementObjects;
        elementObjects.reserve(elementNodes.size());

        Scope tmpScope(scope); /* TODO - evaluate in local scope because will be copied in ArrayObject */

        for (const auto &elementNode : elementNodes)
        {
            elementObjects.push_back(elementNode(tmpScope));
        }

        /* TODO: - really inefficient! Redesign */
        return scope.createManagedObject<ArrayObject>(std::move(elementObjects));
    });
}

} // namespace Instances

namespace Utilities
{
AnyNode createBreakNode()
{
    return AnyNode([](Scope &scope) -> BaseObject *
    {
        jumpToBreakJumpPoint();
        return nullptr;
    });
}

} // namespace Utilities


namespace Operators
{

AnyNode createNotNode(AnyNode body)
{
    return AnyNode([body = std::move(body)](Scope &scope) -> BaseObject *
    {
        BoolObject *result = body.operator()<BoolObject>(scope);

        return scope.createManagedObject<BoolObject>(!(*result));
    });
}

AnyNode createNegationNode(AnyNode body)
{
    return AnyNode([body = std::move(body)](Scope &scope) -> BaseObject *
    {
        BaseObject *result = body(scope);

        if (result->isObjectType<IntObject>())
            return scope.createManagedObject<IntObject>(-result->castObject<IntObject>());
        else if (result->isObjectType<FloatObject>())
            return scope.createManagedObject<FloatObject>(-result->castObject<FloatObject>());

        ThrowException("invalid object type");
    });
}

} // namespace Operators

} // namespace NodeFactory