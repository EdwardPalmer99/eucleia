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

BaseObject *FunctionCallNode::evaluate(Scope &scope)
{
    // 0. Any library functions that we wish to evaluate.
    BaseObject *libraryFunc = scope.getOptionalObject<BaseObject>(funcName->variableName);
    if (libraryFunc && libraryFunc->isObjectType<LibraryFunctionObject>())
    {
        ProgramNode &programNode = (*funcArgs);

        return libraryFunc->castObject<LibraryFunctionObject>().evaluate(programNode, scope);
    }

    // TODO: - finish implementing here. Should not be a shared pointer.
    // 1. Get a pointer to the function node stored in this scope.
    auto funcNode = scope.getObject<FunctionObject>(funcName->variableName)->functionValue;

    // 2. Verify that the number of arguments matches those required for the
    // function we are calling.
    if (funcArgs->programNodes.size() != funcNode->funcArgs->programNodes.size())
    {
        printEucleiaError("expected %ld arguments but got %ld arguments for function '%s'.\n",
                          funcNode->funcArgs->programNodes.size(),
                          funcArgs->programNodes.size(),
                          funcName->variableName.c_str());
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
        auto &argVariable = funcNode->funcArgs->programNodes[iarg++]->castNode<AddNewVariableNode>();

        if (!argVariable.passesAssignmentTypeCheck(*evaluatedArg))
        {
            printEucleiaError("incorrect type for argument '%s' of function '%s'. Expected type '%s'.\n",
                              argVariable.variableName.c_str(),
                              funcName->variableName.c_str(),
                              argVariable.description().c_str());
        }

        // Define variable in the function's scope.
        funcScope.linkObject(argVariable.variableName, evaluatedArg);
    }

    // Evaluate the function body in our function scope now that we've added the
    // call arguments.
    return evaluateFunctionBody(*funcNode->funcBody, funcScope);
}

BaseObject *FunctionCallNode::evaluateFunctionBody(BaseNode &funcBody, Scope &funcScope)
{
    // Reset return value.
    gEnvironmentContext.returnValue = nullptr;

    jmp_buf *original = gEnvironmentContext.returnJumpPoint;

    jmp_buf local;
    gEnvironmentContext.returnJumpPoint = &local;

    // Evaluate each node and return last result.
    if (setjmp(local) != 1)
    {
        funcBody.evaluate(funcScope);
    }

    gEnvironmentContext.returnJumpPoint = original;

    // Only return non-NULL if return seen.
    return gEnvironmentContext.returnValue;
}