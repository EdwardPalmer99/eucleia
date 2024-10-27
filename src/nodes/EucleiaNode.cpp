//
//  ASTBase.cpp
//  Eucleia
//
//  Created by Edward on 14/01/2024.
//

#include "EucleiaNode.hpp"
#include "EucleiaObject.hpp"
#include "EucleiaUtility.hpp"
#include <assert.h>
#include <iostream>
#include <setjmp.h>

struct GlobalEnvRec
{
    jmp_buf *breakJumpPoint;
    jmp_buf *returnJumpPoint;

    std::shared_ptr<BaseObject> returnValue{nullptr};
};

GlobalEnvRec gEnvironmentContext;


#pragma mark - *** Create New Objects ***

/// Create a new BoolObject from a BoolNode.
std::shared_ptr<BaseObject> BoolNode::evaluate(Scope &scope)
{
    return std::make_shared<BoolObject>(boolValue);
}


/// Create a new IntObject from an IntNode.
std::shared_ptr<BaseObject> IntNode::evaluate(Scope &scope)
{
    return std::make_shared<IntObject>(numberValue);
}


/// Create a new FloatObject from a FloatNode.
std::shared_ptr<BaseObject> FloatNode::evaluate(Scope &scope)
{
    return std::make_shared<FloatObject>(numberValue);
}


/// Create a new StringObject from a StringNode.
std::shared_ptr<BaseObject> StringNode::evaluate(Scope &scope)
{
    return std::make_shared<StringObject>(stringValue);
}

/// Create a new ArrayObject from an ArrayNode.
std::shared_ptr<BaseObject> ArrayNode::evaluate(Scope &scope)
{
    std::vector<std::shared_ptr<BaseObject>> evaluatedObjects;

    for (const auto &node : nodes)
    {
        evaluatedObjects.push_back(node->evaluate(scope));
    }

    evaluatedObjects.shrink_to_fit();

    return std::make_shared<ArrayObject>(evaluatedObjects);
}


/// Create a new FunctionObject from a FunctionNode and register in current scope.
std::shared_ptr<BaseObject> FunctionNode::evaluate(Scope &scope)
{
    auto funcNameNode = static_pointer_cast<VariableNode>(funcName);

    auto functionObject = std::make_shared<FunctionObject>(getShared());

    scope.defineObject(funcNameNode->variableName, functionObject);

    return functionObject;
}


#pragma mark -

/// Evaluate the sequence of nodes stored in a ProgramNode.
std::shared_ptr<BaseObject> ProgramNode::evaluate(Scope &scope)
{
    // Create inner program scope for each block of statements. Good example is
    // for a loop where the body of the loop redeclares a variable. This should
    // be okay.
    Scope programScope(scope);

    for (const auto &node : nodes)
    {
        node->evaluate(programScope);
    }

    return nullptr; // Nothing returned.
}


/// Evaluates a file. Does NOT create an inner scope node in order to ensure that
/// any functions declared in this file will be added to the master file by
/// using the same global scope.
std::shared_ptr<BaseObject> FileNode::evaluate(Scope &globalScope)
{
    for (const auto &node : nodes)
    {
        node->evaluate(globalScope);
    }

    return nullptr;
}


/// Return the object associated with the variable name in the scope. This could
/// be any of the core BaseObject types.
std::shared_ptr<BaseObject> VariableNameNode::evaluate(Scope &scope)
{
    return scope.getObject(variableName);
}


/// Creates a new uninitialized variable and defines it in the current scope.
std::shared_ptr<BaseObject> VariableNode::evaluate(Scope &scope)
{
    std::shared_ptr<BaseObject> variableObject{nullptr};

    switch (variableType)
    {
        case Int:
            variableObject = std::make_shared<IntObject>(0);
            break;
        case Float:
            variableObject = std::make_shared<FloatObject>(0.0);
            break;
        case Bool:
            variableObject = std::make_shared<BoolObject>(false);
            break;
        case String:
            variableObject = std::make_shared<StringObject>("");
            break;
        case Array:
            variableObject = std::make_shared<ArrayObject>(std::vector<std::shared_ptr<BaseObject>>());
            break;
            // TODO: - handle function type. (will need to think about this.)
        default:
            break;
    }

    scope.defineObject(variableName, variableObject);

    return variableObject;
}


/// Evaluate the if statement.
std::shared_ptr<BaseObject> IfNode::evaluate(Scope &scope)
{
    auto condition = ifCondition->evaluate<BoolObject>(scope);

    if (condition->value)
        return thenDo->evaluate(scope);
    else if (elseDo != nullptr) // Optional "else"
        return elseDo->evaluate(scope);
    else
        return nullptr;
}


#pragma mark -


std::shared_ptr<BaseObject> BinaryNode::evaluate(Scope &scope)
{
    auto leftEvaluated = left->evaluate(scope);
    auto rightEvaluated = right->evaluate(scope);

    return applyOperator(std::move(leftEvaluated), std::move(rightEvaluated));
}


std::shared_ptr<BaseObject> AssignNode::evaluate(Scope &scope)
{
    // Special case; example: array[0] = 3;
    if (left->type() == NodeType::ArrayAccess)
    {
        return evaluateArrayAccess(scope);
    }

    // 1. Cast LHS to a variable node or a variable name node.
    assert(left->type() == NodeType::VariableName ||
           left->type() == NodeType::Variable);

    // Evaluate the LHS.
    // Case 1: VariableName node --> returns shared pointer to existing object (not useful).
    // Case 2: Variable node --> creates new object in scope.
    if (left->type() == NodeType::Variable)
    {
        left->evaluate(scope);
    }

    // 2. Evaluate the right-hand-side.
    auto rightEvaluated = right->evaluate(scope);

    // 3. Update value of variable object.
    auto leftVariableName = static_pointer_cast<VariableNameNode>(left);
    scope.updateObject(leftVariableName->variableName, rightEvaluated);

    return nullptr;
}


#pragma mark -

std::shared_ptr<BaseObject> BreakNode::evaluate(Scope &scope)
{
    longjmp(*gEnvironmentContext.breakJumpPoint, 1);
    return nullptr;
}


std::shared_ptr<BaseObject> ReturnNode::evaluate(Scope &scope)
{
    if (returnNode != nullptr) // i.e. return true;
    {
        gEnvironmentContext.returnValue = returnNode->evaluate(scope);
    }
    else
    {
        gEnvironmentContext.returnValue = nullptr;
    }

    longjmp(*gEnvironmentContext.returnJumpPoint, 1);
    return NULL;
}


#pragma mark -

std::shared_ptr<BaseObject> WhileNode::evaluate(Scope &scope)
{
    // Set jump point for break statements.
    jmp_buf *original = gEnvironmentContext.breakJumpPoint;

    jmp_buf local;
    gEnvironmentContext.breakJumpPoint = &local;

    if (setjmp(local) != 1)
    {
        Scope loopScope(scope); // Extend scope.

        while (condition->evaluate<BoolObject>(scope)->value)
        {
            body->evaluate(loopScope);
        }
    }

    // Restore original context.
    gEnvironmentContext.breakJumpPoint = original;

    return nullptr;
}

std::shared_ptr<BaseObject> DoWhileNode::evaluate(Scope &scope)
{
    jmp_buf *original = gEnvironmentContext.breakJumpPoint;

    jmp_buf local;
    gEnvironmentContext.breakJumpPoint = &local;

    if (setjmp(local) != 1)
    {
        Scope loopScope(scope); // Extend scope.

        do
        {
            body->evaluate(loopScope);
        } while (condition->evaluate<BoolObject>(scope)->value);
    }

    // Restore original context.
    gEnvironmentContext.breakJumpPoint = original;

    return nullptr; // Return nothing.
}

std::shared_ptr<BaseObject> ForLoopNode::evaluate(Scope &scope)
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
             condition->evaluate<BoolObject>(loopScope)->value;
             update->evaluate(loopScope))
        {
            body->evaluate(loopScope);
        }
    }

    gEnvironmentContext.breakJumpPoint = original;

    return nullptr;
}


std::shared_ptr<BaseObject> FunctionCallNode::evaluateFunctionBody(BaseNode &funcBody, Scope &funcScope)
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

std::shared_ptr<BaseObject> FunctionCallNode::evaluate(Scope &scope)
{
    // 0. Any library functions that we wish to evaluate.
    auto libraryFunc = scope.getOptionalObject<LibraryFunctionObject>(funcName->variableName);
    if (libraryFunc)
    {
        return libraryFunc->evaluate(*funcArgs, scope);
    }

    // 1. Get a shared pointer to the function node stored in this scope.
    auto funcNode = scope.getObject<FunctionObject>(funcName->variableName)->functionValue;

    // 2. Verify that the number of arguments matches those required for the
    // function we are calling.
    if (funcArgs->nodes.size() != funcNode->funcArgs->nodes.size())
    {
        printWarpError("expected %ld arguments but got %ld arguments for function '%s'.\n",
                       funcNode->funcArgs->nodes.size(),
                       funcArgs->nodes.size(),
                       funcName->variableName.c_str());
    }

    // 3. Extend current scope (outside function) with names and values of function
    // arguments.
    auto funcScope(scope);

    // TODO: - evaluate all of the function's parameters in function scope to create uninitialized variables.
    // THen call setObject with all of the arguments to update the values and our type-checker will ensure
    // that the object types are compatible.

    int iarg = 0;
    for (const auto &argNode : funcArgs->nodes)
    {
        // Evaluate all function arguments in external scope (outside function).
        auto evaluatedArg = argNode->evaluate(scope);

        // Check that the evaluatedArg type (RHS) is compatible with the corresponding
        // (LHS) variable.
        auto argVariable = std::static_pointer_cast<VariableNode>(funcNode->funcArgs->nodes[iarg++]);

        if (!argVariable->passesAssignmentTypeCheck(*evaluatedArg))
        {
            printWarpError("incorrect type for argument '%s' of function '%s'. Expected type '%s'.\n",
                           argVariable->variableName.c_str(),
                           funcName->variableName.c_str(),
                           argVariable->description().c_str());
        }

        // Define variable in the function's scope.
        funcScope.defineObject(argVariable->variableName, evaluatedArg);
    }

    // Evaluate the function body in our function scope now that we've added the
    // call arguments.
    return evaluateFunctionBody(*funcNode->funcBody, funcScope);
}


#pragma mark -

/// Special method called if LHS is an array accessor.
std::shared_ptr<BaseObject> AssignNode::evaluateArrayAccess(Scope &scope)
{
    assert(left->type() == NodeType::ArrayAccess);

    // 1. Get the array object at that index.
    auto currentValue = left->evaluate(scope);

    // 2. Evaluate RHS to get new value for the object.
    auto newValue = right->evaluate(scope);

    // 3. Check that the types match.
    assert(currentValue->type() == newValue->type());

    // Set to new value.
    *currentValue = *newValue;

    return nullptr;
}


std::shared_ptr<BaseObject> ArrayAccessNode::evaluate(Scope &scope)
{
    // Get shared pointer to array.
    std::shared_ptr<BaseObject> evaluatedObject = arrayName->evaluate(scope);
    assert(evaluatedObject->type() == BaseObject::ObjectType::Array);

    auto array = std::static_pointer_cast<ArrayObject>(evaluatedObject);

    const long index = arrayIndex->numberValue;
    assert(index >= 0 && index < array->arrayValues.size());

    return (array->arrayValues.at(index));
}


#pragma mark - *** Helper ***

std::shared_ptr<BaseObject> BinaryNode::applyOperator(std::shared_ptr<BaseObject> left, std::shared_ptr<BaseObject> right)
{
    // Require both types to be integers to perform integer operations.
    bool intOperations = (left->type() == BaseObject::ObjectType::Int &&
                          right->type() == BaseObject::ObjectType::Int);

    if (binaryOperator == "+")
    {
        if (intOperations)
            return std::make_shared<IntObject>(left->intCast() + right->intCast());
        else
            return std::make_shared<FloatObject>(left->floatCast() + right->floatCast());
    }
    else if (binaryOperator == "-")
    {
        if (intOperations)
            return std::make_shared<IntObject>(left->intCast() - right->intCast());
        else
            return std::make_shared<FloatObject>(left->floatCast() - right->floatCast());
    }
    else if (binaryOperator == "*")
    {
        if (intOperations)
            return std::make_shared<IntObject>(left->intCast() * right->intCast());
        else
            return std::make_shared<FloatObject>(left->floatCast() * right->floatCast());
    }
    else if (binaryOperator == "/")
    {
        if (intOperations)
            return std::make_shared<IntObject>(left->intCast() / castToInt(*right).nonzeroValue());
        else
            return std::make_shared<FloatObject>(left->floatCast() / right->floatCast());
    }
    else if (binaryOperator == "%")
    {
        return std::make_shared<IntObject>(castToInt(*left).positiveOrZeroValue() / castToInt(*right).positiveValue());
    }
    else if (binaryOperator == "&&")
    {
        return std::make_shared<BoolObject>(left->boolCast() && right->boolCast());
    }
    else if (binaryOperator == "||")
    {
        return (left->boolCast() ? left : right);
    }
    else if (binaryOperator == "<")
    {
        return std::make_shared<BoolObject>(left->floatCast() < right->floatCast());
    }
    else if (binaryOperator == ">")
    {
        return std::make_shared<BoolObject>(left->floatCast() > right->floatCast());
    }
    else if (binaryOperator == "<=")
    {
        return std::make_shared<BoolObject>(left->floatCast() <= right->floatCast());
    }
    else if (binaryOperator == ">=")
    {
        return std::make_shared<BoolObject>(left->floatCast() >= right->floatCast());
    }
    else if (binaryOperator == "==")
    {
        return std::make_shared<BoolObject>(left->floatCast() == right->floatCast());
    }
    else if (binaryOperator == "!=") // TODO: - need some tolerance.
    {
        return std::make_shared<BoolObject>(left->floatCast() != right->floatCast());
    }
    else
    {
        printWarpError("cannot apply operator %s.\n", binaryOperator.c_str());
    }
}


#pragma mark - *** Variable Type Checking ***

/// Type checking.
bool VariableNode::passesAssignmentTypeCheck(const BaseObject &assignObject) const
{
    auto assignObjectType = assignObject.type();

    switch (variableType)
    {
        case Int:
            return (assignObjectType == BaseObject::ObjectType::Int);
        case Float:
            return (assignObjectType == BaseObject::ObjectType::Float);
        case Bool:
            return (assignObjectType == BaseObject::ObjectType::Bool);
        case String:
            return (assignObjectType == BaseObject::ObjectType::String);
        case Array:
            return (assignObjectType == BaseObject::ObjectType::Array);
        default:
            return false;
    }
}


std::string VariableNode::description() const
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
