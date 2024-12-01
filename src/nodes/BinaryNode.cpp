/**
 * @file BinaryNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "BinaryNode.hpp"

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
        EucleiaError("cannot apply operator '%s' to types Int, Int.", binaryOperator.c_str());
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
        EucleiaError("cannot apply operator '%s' to types Int, Int.", binaryOperator.c_str());
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
        EucleiaError("cannot apply operator '%s' to types String, String.", binaryOperator.c_str());
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
        EucleiaError("cannot apply operator '%s' to object types.", binaryOperator.c_str());
    }
}