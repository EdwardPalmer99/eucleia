/**
 * @file FunctionCallNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "FunctionCallNode.hpp"
#include "FunctionNode.hpp"
#include "FunctionObject.hpp"
#include "JumpPoints.hpp"
#include "ModuleFunctionObject.hpp"

BaseObject::Ptr FunctionCallNode::evaluate(Scope &scope)
{
    // 0. Any library functions that we wish to evaluate.
    auto libraryFunc = scope.getOptionalNamedObject<BaseObject>(_funcName);
    if (libraryFunc && libraryFunc->isObjectType<ModuleFunctionObject>())
    {
        return libraryFunc->castObject<ModuleFunctionObject>()(_funcArgs, scope);
    }

    // TODO: - finish implementing here. Should not be a shared pointer.
    // 1. Get a pointer to the function node stored in this scope.
    auto funcNode = scope.getNamedObject<FunctionObject>(_funcName)->value();

    // 2. Verify that the number of arguments matches those required for the
    // function we are calling.
    if (_funcArgs.size() != funcNode->_funcArgs.size())
    {
        char buffer[150];
        snprintf(buffer, 150, "expected %ld arguments but got %ld arguments for function '%s'.",
                 funcNode->_funcArgs.size(),
                 _funcArgs.size(),
                 _funcName.c_str());

        ThrowException(buffer);
    }

    // 3. Extend current scope (outside function) with names and values of function
    // arguments.
    auto funcScope(scope);

    // TODO: - evaluate all of the function's parameters in function scope to create uninitialized variables.
    // THen call setObject with all of the arguments to update the values and our type-checker will ensure
    // that the object types are compatible.

    int iarg = 0;
    for (const auto &argNode : _funcArgs)
    {
        // Evaluate all function arguments in external scope (outside function).
        auto evaluatedArg = argNode->evaluate(scope);

        // Check that the evaluatedArg type (RHS) is compatible with the corresponding
        // (LHS) variable.
        auto &argVariable = funcNode->_funcArgs[iarg++]->castNode<AddVariableNode>();

        if (!argVariable.passesAssignmentTypeCheck(*evaluatedArg))
        {
            char buffer[150];
            snprintf(buffer, 150, "incorrect type for argument '%s' of function '%s'. Expected type '%s'.",
                     argVariable.name().c_str(),
                     _funcName.c_str(),
                     argVariable.description().c_str());

            ThrowException(buffer);
        }

        // Define variable in the function's scope.
        funcScope.linkObject(argVariable.name(), evaluatedArg);
    }

    // Evaluate the function body in our function scope now that we've added the
    // call arguments.
    return evaluateFunctionBody(*funcNode->funcBody, funcScope);
}

BaseObject::Ptr FunctionCallNode::evaluateFunctionBody(BaseNode &funcBody, Scope &funcScope)
{
    // Reset return value.
    gEnvironmentContext.returnValue = nullptr;

    jmp_buf *original = gEnvironmentContext.returnJumpPoint;

    jmp_buf local;
    gEnvironmentContext.returnJumpPoint = &local;

    // Evaluate each node and return last result.
    if (setjmp(local) != 1)
    {
        (void)funcBody.evaluate(funcScope);
    }

    gEnvironmentContext.returnJumpPoint = original;

    // Only return non-NULL if return seen.
    return gEnvironmentContext.returnValue;
}