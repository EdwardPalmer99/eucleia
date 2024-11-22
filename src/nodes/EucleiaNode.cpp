//
//  ASTBase.cpp
//  Eucleia
//
//  Created by Edward on 14/01/2024.
//

#include "EucleiaNode.hpp"
#include "EucleiaUtility.hpp"
#include "ExpressionScope.hpp"
#include "Objects.hpp"
#include <assert.h>
#include <iostream>
#include <setjmp.h>

struct GlobalEnvRec
{
    jmp_buf *breakJumpPoint;
    jmp_buf *returnJumpPoint;

    BaseObject *returnValue{nullptr};
};

GlobalEnvRec gEnvironmentContext;


#pragma mark - *** Create New Objects ***


ArrayObject *AddArrayNode::evaluate(Scope &scope)
{
    std::vector<BaseObject *> evaluatedObjects;

    for (auto *node : programNodes)
    {
        evaluatedObjects.push_back(node->evaluate(scope));
    }

    evaluatedObjects.shrink_to_fit();

    return scope.createManagedObject<ArrayObject>(evaluatedObjects);
}


/// Create a new FunctionObject from a FunctionNode and register in current scope.
BaseObject *FunctionNode::evaluate(Scope &scope)
{
    auto &funcNameNode = funcName->castNode<AddNewVariableNode>();

    auto functionObject = scope.createManagedObject<FunctionObject>(this); // TODO: - uh-oh!! Think about pointer ownership here!!

    scope.defineObject(funcNameNode.variableName, functionObject);

    return functionObject;
}


#pragma mark -

BaseObject *ProgramNode::evaluate(Scope &scope)
{
    // Create inner program scope for each block of statements. Good example is
    // for a loop where the body of the loop redeclares a variable. This should
    // be okay.
    Scope programScope(scope);

    for (const auto &node : programNodes)
    {
        node->evaluate(programScope);
    }

    // Any memory allocations cleared-up when programScope variable goes out of scope.

    return nullptr; // Nothing returned.
}

BaseObject *FileNode::evaluate(Scope &globalScope)
{
    for (const auto &node : programNodes)
    {
        node->evaluate(globalScope);
    }

    return nullptr;
}


/// Return the object associated with the variable name in the scope. This could
/// be any of the core BaseObject types.
BaseObject *LookupVariableNode::evaluate(Scope &scope)
{
    return scope.getDefinedObject(variableName);
}


/// Creates a new uninitialized variable and defines it in the current scope.
BaseObject *AddNewVariableNode::evaluate(Scope &scope)
{
    BaseObject *objectPtr = nullptr;

    switch (variableType)
    {
        case Int:
        case Bool:
            objectPtr = scope.createManagedObject<IntObject>();
            break;
        case Float:
            objectPtr = scope.createManagedObject<FloatObject>();
            break;
        case String:
            objectPtr = scope.createManagedObject<StringObject>();
            break;
        case Array:
            objectPtr = scope.createManagedObject<ArrayObject>();
            break;
        // TODO: - handle function type. (will need to think about this.)
        default:
            printEucleiaError("%s", "cannot create a variable of specified type.");
    }

    scope.defineObject(variableName, objectPtr); // TODO: - could call this linkObjectToVariableName()
    return objectPtr;
}


BaseObject *IfNode::evaluate(Scope &scope)
{
    // NB: the condition should be evaluated in its own scope as it is within ()
    // brackets. We don't want it to persist once evaluated.
    if (evaluate<BoolObject>(ifCondition, scope).value)
        return thenDo->evaluate(scope);
    else if (elseDo != nullptr) // Optional "else"
        return elseDo->evaluate(scope);
    else
        return nullptr;
}


#pragma mark -


BaseObject *BinaryNode::evaluate(Scope &scope)
{
    // TODO: - lose a lot of performance. Scope class is too heavy!!!!

    // Evaluate left/right in inner scope so destroyed after evaluated.
    Scope binaryScope(scope);

    auto leftEvaluated = left->evaluate(binaryScope);
    auto rightEvaluated = right->evaluate(binaryScope);

    // Persist result by storing in outer scope.
    return applyOperator(scope, *leftEvaluated, *rightEvaluated);
}


BaseObject *AssignNode::evaluate(Scope &scope)
{
    // Special case; example: array[0] = 3;
    if (left->isNodeType<ArrayAccessNode>())
    {
        return evaluateArrayAccess(scope);
    }

    // 1. Cast LHS to a variable node or a variable name node.
    assert(left->isNodeType<AddNewVariableNode>() || left->isNodeType<LookupVariableNode>());

    // Evaluate the LHS.
    // Case 1: VariableName node --> returns shared pointer to existing object (not useful).
    // Case 2: Variable node --> creates new object in scope.
    if (left->isNodeType<AddNewVariableNode>())
    {
        (void)left->evaluate(scope);
    }

    // 2. Evaluate the right-hand-side.
    auto rightEvaluated = right->evaluate(scope);

    // 3. Update value of variable object.
    auto &leftVariableName = left->castNode<LookupVariableNode>();
    scope.updateObject(leftVariableName.variableName, rightEvaluated);

    return nullptr;
}


#pragma mark -

BaseObject *BreakNode::evaluate(Scope &scope)
{
    longjmp(*gEnvironmentContext.breakJumpPoint, 1);
    return nullptr;
}


BaseObject *ReturnNode::evaluate(Scope &scope)
{
    gEnvironmentContext.returnValue = nullptr;

    if (returnNode != nullptr) // i.e. return true;
    {
        // Evaluate in function scope (need all local variables, etc.).
        BaseObject *tmpResult = returnNode->evaluate(scope);
        if (tmpResult != nullptr)
        {
            // TODO: - instead, remove this from the scope and copy into parent scope instead of cloning.

            // Get the scope in which the function was called. We copy the object to this scope.
            // This is because the function scope will destroy the return object as soon as its
            // destructor is called and we need this object to persist until parent scope destructor called.
            assert(scope.parentScope() != nullptr);

            BaseObject *result = scope.parentScope()->cloneObject(tmpResult);
            gEnvironmentContext.returnValue = result;
        }
    }

    longjmp(*gEnvironmentContext.returnJumpPoint, 1);
    return nullptr;
}


#pragma mark -

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

BaseObject *DoWhileNode::evaluate(Scope &scope)
{
    jmp_buf *original = gEnvironmentContext.breakJumpPoint;

    jmp_buf local;
    gEnvironmentContext.breakJumpPoint = &local;

    if (setjmp(local) != 1)
    {
        Scope loopScope(scope); // Extend scope.

        do
        {
            (void)body->evaluate(loopScope);
        } while (evaluate<BoolObject>(condition, scope).value);
    }

    // Restore original context.
    gEnvironmentContext.breakJumpPoint = original;

    return nullptr; // Return nothing.
}

BaseObject *ForLoopNode::evaluate(Scope &scope)
{
    jmp_buf *original = gEnvironmentContext.breakJumpPoint;

    // Initialization.
    Scope loopScope(scope); // Extend scope.

    start->evaluate(loopScope);

    jmp_buf local;
    gEnvironmentContext.breakJumpPoint = &local;

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

    gEnvironmentContext.breakJumpPoint = original;

    return nullptr;
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


#pragma mark -

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
        funcScope.defineObject(argVariable.variableName, evaluatedArg);
    }

    // Evaluate the function body in our function scope now that we've added the
    // call arguments.
    return evaluateFunctionBody(*funcNode->funcBody, funcScope);
}


#pragma mark -

/// Special method called if LHS is an array accessor.
BaseObject *AssignNode::evaluateArrayAccess(Scope &scope)
{
    assert(left->isNodeType<ArrayAccessNode>());

    // 1. Get the array object at that index.
    auto currentValue = left->evaluate(scope);

    // 2. Evaluate RHS to get new value for the object.
    auto newValue = right->evaluate(scope);

    // 3. Check that the types match.
    assert(currentValue->typesMatch(*newValue));

    // Set to new value.
    *currentValue = *newValue;
    printEucleiaError("%s", "WIP: LOGIC IS NOT CORRECT. THINK ABOUT HOW TO REMOVE OLD VALUE FROM SCOPE.");

    return nullptr;
}


BaseObject *ArrayAccessNode::evaluate(Scope &scope)
{
    printEucleiaError("%s", "Not implemented. Think abouto this...");

    // // Get shared pointer to array.
    // ArrayObject *evaluatedObject = arrayName->evaluate(scope);

    // auto arrayObject = evaluatedObject->castObject<ArrayObject>();

    // const long index = arrayIndex->numberValue;
    // return arrayObject[index];
}


#pragma mark - *** Helper ***

BaseObject *BinaryNode::applyOperator(Scope &scope, const IntObject &left, const IntObject &right) const
{
    if (binaryOperator == "+")
        return scope.createManagedObject<IntObject>(left + right);
    else if (binaryOperator == "-")
        return scope.createManagedObject<IntObject>(left - right);
    else if (binaryOperator == "*")
        return scope.createManagedObject<IntObject>(left * right);
    else if (binaryOperator == "/")
        return scope.createManagedObject<IntObject>(left / right);
    else if (binaryOperator == "==")
        return scope.createManagedObject<BoolObject>(left == right);
    else if (binaryOperator == "!=")
        return scope.createManagedObject<BoolObject>(left != right);
    else if (binaryOperator == ">=")
        return scope.createManagedObject<BoolObject>(left >= right);
    else if (binaryOperator == ">")
        return scope.createManagedObject<BoolObject>(left > right);
    else if (binaryOperator == "<=")
        return scope.createManagedObject<BoolObject>(left <= right);
    else if (binaryOperator == "<")
        return scope.createManagedObject<BoolObject>(left < right);
    else if (binaryOperator == "%")
        return scope.createManagedObject<IntObject>(left % right);
    else if (binaryOperator == "&&")
        return scope.createManagedObject<BoolObject>(left && right);
    else if (binaryOperator == "||")
        return scope.createManagedObject<BoolObject>(left || right);
    else
        printEucleiaError("cannot apply operator '%s' to types Int, Int.\n", binaryOperator.c_str());
}


BaseObject *BinaryNode::applyOperator(Scope &scope, const FloatObject &left, const FloatObject &right) const
{
    if (binaryOperator == "+")
        return scope.createManagedObject<FloatObject>(left + right);
    else if (binaryOperator == "-")
        return scope.createManagedObject<FloatObject>(left - right);
    else if (binaryOperator == "*")
        return scope.createManagedObject<FloatObject>(left * right);
    else if (binaryOperator == "/")
        return scope.createManagedObject<FloatObject>(left / right);
    else if (binaryOperator == "==")
        return scope.createManagedObject<BoolObject>(left == right);
    else if (binaryOperator == "!=")
        return scope.createManagedObject<BoolObject>(left != right);
    else if (binaryOperator == ">=")
        return scope.createManagedObject<BoolObject>(left >= right);
    else if (binaryOperator == ">")
        return scope.createManagedObject<BoolObject>(left > right);
    else if (binaryOperator == "<=")
        return scope.createManagedObject<BoolObject>(left <= right);
    else if (binaryOperator == "<")
        return scope.createManagedObject<BoolObject>(left < right);
    else
        printEucleiaError("cannot apply operator '%s' to types Int, Int.\n", binaryOperator.c_str());
}


BaseObject *BinaryNode::applyOperator(Scope &scope, const StringObject &left, const StringObject &right) const
{
    if (binaryOperator == "+")
        return scope.createManagedObject<StringObject>(left + right);
    else if (binaryOperator == "==")
        return scope.createManagedObject<BoolObject>(left == right);
    else if (binaryOperator == "!=")
        return scope.createManagedObject<BoolObject>(left != right);
    else
        printEucleiaError("cannot apply operator '%s' to types String, String.\n", binaryOperator.c_str());
}


BaseObject *BinaryNode::applyOperator(Scope &scope, const BaseObject &left, const BaseObject &right) const
{
    if (left.isObjectType<IntObject>() && right.isObjectType<IntObject>())
    {
        return applyOperator(scope, left.castObject<IntObject>(), right.castObject<IntObject>());
    }
    else if (left.isObjectType<FloatObject>() && right.isObjectType<FloatObject>())
    {
        return applyOperator(scope, left.castObject<FloatObject>(), right.castObject<FloatObject>());
    }
    else if (left.isObjectType<IntObject>() && right.isObjectType<FloatObject>())
    {
        return applyOperator(scope, left.castObject<IntObject>().castToFloat(), right.castObject<FloatObject>());
    }
    else if (left.isObjectType<FloatObject>() && right.isObjectType<IntObject>())
    {
        return applyOperator(scope, left.castObject<FloatObject>(), right.castObject<IntObject>().castToFloat());
    }
    else if (left.isObjectType<StringObject>() && right.isObjectType<StringObject>())
    {
        return applyOperator(scope, left.castObject<StringObject>(), right.castObject<StringObject>());
    }
    else
    {
        printEucleiaError("Cannot apply operator '%s' to object types.\n", binaryOperator.c_str());
    }
}


#pragma mark - *** Variable Type Checking ***

/// Type checking.
bool AddNewVariableNode::passesAssignmentTypeCheck(const BaseObject &assignObject) const
{
    switch (variableType)
    {
        case Int:
            return assignObject.isObjectType<IntObject>();
        case Float:
            return assignObject.isObjectType<FloatObject>();
        case Bool:
            return assignObject.isObjectType<BoolObject>();
        case String:
            return assignObject.isObjectType<StringObject>();
        case Array:
            return assignObject.isObjectType<ArrayObject>();
        default:
            return false;
    }
}


std::string AddNewVariableNode::description() const
{
    switch (variableType)
    {
        case Bool:
            return "Bool";
        case Int:
            return "Int";
        case Float:
            return "Float";
        case String:
            return "String";
        case Array:
            return "Array";
        default:
            return "Unknown";
    }
}
