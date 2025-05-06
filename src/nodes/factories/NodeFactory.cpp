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

AnyNode createArrayNode(AnyNodeVector elementNodes)
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


AnyNode createBlockNode(AnyNodeVector nodes)
{
    return AnyNode([nodes = std::move(nodes)](Scope &scope) -> BaseObject *
    {
        /*
         * Create inner program scope for each block of statements. Good example is for a loop where the body of the
         * loop redeclares a variable. This should be okay.
         */
        Scope programScope(scope);

        for (const auto &node : nodes)
        {
            (void)node(programScope);
        }

        /* Any memory allocations cleared-up when programScope variable goes out of scope */

        return nullptr; /* Nothing returned */
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
        BoolObject &result = body.operator()<BoolObject>(scope);

        return scope.createManagedObject<BoolObject>(!result);
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

#include "StringObject.hpp"

namespace Functions
{

AnyNode createFunctionCallNode(AnyNode funcName, AnyNode funcArgs)
{
    return AnyNode([funcName = std::move(funcName), funcArgs = std::move(funcArgs)](Scope &scope) -> BaseObject *
    {
        const std::string &theFuncName = *funcName.operator()<StringObject>(scope);
        const ArrayObject &theFuncArgs = funcArgs.operator()<ArrayObject>(scope);

        // 0. Any library functions that we wish to evaluate.
        BaseObject *libraryFunc = scope.getOptionalNamedObject<BaseObject>(theFuncName);
        if (libraryFunc && libraryFunc->isObjectType<LibraryFunctionObject>())
        {
            return libraryFunc->castObject<LibraryFunctionObject>()(theFuncArgs, scope);
        }

        // TODO: - finish implementing here. Should not be a shared pointer.
        // 1. Get a pointer to the function node stored in this scope.
        auto funcNode = scope.getNamedObject<FunctionObject>(theFuncName)->value();

        // 2. Verify that the number of arguments matches those required for the
        // function we are calling.
        if (theFuncArgs.value().size() != funcNode->funcArgs->programNodes.size())
        {
            char buffer[150];
            snprintf(buffer, 150, "expected %ld arguments but got %ld arguments for function '%s'.",
                     funcNode->funcArgs->programNodes.size(),
                     theFuncArgs.value().size(),
                     theFuncName.c_str());

            ThrowException(buffer);
        }

        // 3. Extend current scope (outside function) with names and values of function
        // arguments.
        auto funcScope(scope);

        // TODO: - evaluate all of the function's parameters in function scope to create uninitialized variables.
        // THen call setObject with all of the arguments to update the values and our type-checker will ensure
        // that the object types are compatible.

        int iarg = 0;
        for (const auto &argNode : funcArgs->programNodes)
        {
            // Evaluate all function arguments in external scope (outside function).
            auto evaluatedArg = argNode->evaluate(scope);

            // Check that the evaluatedArg type (RHS) is compatible with the corresponding
            // (LHS) variable.
            auto &argVariable = funcNode->funcArgs->programNodes[iarg++]->castNode<AddVariableNode>();

            if (!argVariable.passesAssignmentTypeCheck(*evaluatedArg))
            {
                char buffer[150];
                snprintf(buffer, 150, "incorrect type for argument '%s' of function '%s'. Expected type '%s'.",
                         argVariable.name.c_str(),
                         funcName->name.c_str(),
                         argVariable.description().c_str());

                ThrowException(buffer);
            }

            // Define variable in the function's scope.
            funcScope.linkObject(argVariable.name, evaluatedArg);
        }

        // Evaluate the function body in our function scope now that we've added the
        // call arguments.
        return evaluateFunctionBody(*funcNode->funcBody, funcScope);
    });
}

AnyNode createFunctionNode(AnyNode funcName, AnyNode funcArgs, AnyNode funcBody)
{
}

} // namespace Functions

namespace Files
{

AnyNode createFileNode(AnyNodeVector fileNodes)
{
    return AnyNode([fileNodes = std::move(fileNodes)](Scope &globalScope) -> BaseObject *
    {
        for (const auto &node : fileNodes)
        {
            (void)node(globalScope);
        }

        return nullptr;
    });
}


} // namespace Files

} // namespace NodeFactory