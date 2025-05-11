/**
 * @file BinaryNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "BinaryNode.hpp"
#include "ArrayObject.hpp"

BaseObject *BinaryNode::evaluate(Scope &scope)
{
    // TODO: - lose a lot of performance. Scope class is too heavy!!!!

    // Evaluate left/right in inner scope so destroyed after evaluated.
    Scope binaryScope(scope);

    auto leftEvaluated = _left->evaluate(binaryScope);
    auto rightEvaluated = _right->evaluate(binaryScope);

    // Persist result by storing in outer scope.
    return applyOperator(scope, *leftEvaluated, *rightEvaluated);
}


BaseObject *BinaryNode::applyOperator(Scope &scope, const IntObject &left, const IntObject &right) const
{
    if (_binaryOperator == "+")
        return scope.createManagedObject<IntObject>(left + right);
    else if (_binaryOperator == "-")
        return scope.createManagedObject<IntObject>(left - right);
    else if (_binaryOperator == "*")
        return scope.createManagedObject<IntObject>(left * right);
    else if (_binaryOperator == "/")
        return scope.createManagedObject<IntObject>(left / right);
    else if (_binaryOperator == "==")
        return scope.createManagedObject<BoolObject>(left == right);
    else if (_binaryOperator == "!=")
        return scope.createManagedObject<BoolObject>(left != right);
    else if (_binaryOperator == ">=")
        return scope.createManagedObject<BoolObject>(left >= right);
    else if (_binaryOperator == ">")
        return scope.createManagedObject<BoolObject>(left > right);
    else if (_binaryOperator == "<=")
        return scope.createManagedObject<BoolObject>(left <= right);
    else if (_binaryOperator == "<")
        return scope.createManagedObject<BoolObject>(left < right);
    else if (_binaryOperator == "%")
        return scope.createManagedObject<IntObject>(left % right);
    else if (_binaryOperator == "&&")
        return scope.createManagedObject<BoolObject>(left && right);
    else if (_binaryOperator == "||")
        return scope.createManagedObject<BoolObject>(left || right);
    else
        ThrowException("cannot apply operator " + _binaryOperator + " to types Int, Int");
}


BaseObject *BinaryNode::applyOperator(Scope &scope, const FloatObject &left, const FloatObject &right) const
{
    if (_binaryOperator == "+")
        return scope.createManagedObject<FloatObject>(left + right);
    else if (_binaryOperator == "-")
        return scope.createManagedObject<FloatObject>(left - right);
    else if (_binaryOperator == "*")
        return scope.createManagedObject<FloatObject>(left * right);
    else if (_binaryOperator == "/")
        return scope.createManagedObject<FloatObject>(left / right);
    else if (_binaryOperator == "==")
        return scope.createManagedObject<BoolObject>(left == right);
    else if (_binaryOperator == "!=")
        return scope.createManagedObject<BoolObject>(left != right);
    else if (_binaryOperator == ">=")
        return scope.createManagedObject<BoolObject>(left >= right);
    else if (_binaryOperator == ">")
        return scope.createManagedObject<BoolObject>(left > right);
    else if (_binaryOperator == "<=")
        return scope.createManagedObject<BoolObject>(left <= right);
    else if (_binaryOperator == "<")
        return scope.createManagedObject<BoolObject>(left < right);
    else
        ThrowException("cannot apply operator " + _binaryOperator + " to types Int, Int");
}


BaseObject *BinaryNode::applyOperator(Scope &scope, const StringObject &left, const StringObject &right) const
{
    if (_binaryOperator == "+")
        return scope.createManagedObject<StringObject>(left + right);
    else if (_binaryOperator == "==")
        return scope.createManagedObject<BoolObject>(left == right);
    else if (_binaryOperator == "!=")
        return scope.createManagedObject<BoolObject>(left != right);
    else
        ThrowException("cannot apply operator " + _binaryOperator + " to types String, String");
}


BaseObject *BinaryNode::applyOperator(Scope &scope, const BaseObject &left, const BaseObject &right) const
{
    // TODO: - implement + operator for other object types. Will mean we don't need any if/else statements.

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
    else if (left.isObjectType<ArrayObject>() && right.isObjectType<ArrayObject>())
    {
        return scope.addManagedObject(left + right);
    }
    else
    {
        ThrowException("cannot apply operator " + _binaryOperator + " to object types");
    }
}