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
#include <iostream>


BinaryOperatorType binaryOperatorType(const std::string &operatorString)
{
    if (operatorString == "+")
        return BinaryOperatorType::Add;
    else if (operatorString == "-")
        return BinaryOperatorType::Minus;
    else if (operatorString == "*")
        return BinaryOperatorType::Multiply;
    else if (operatorString == "/")
        return BinaryOperatorType::Divide;
    else if (operatorString == "==")
        return BinaryOperatorType::Equal;
    else if (operatorString == "!=")
        return BinaryOperatorType::NotEqual;
    else if (operatorString == ">=")
        return BinaryOperatorType::GreaterOrEqual;
    else if (operatorString == ">")
        return BinaryOperatorType::Greater;
    else if (operatorString == "<=")
        return BinaryOperatorType::LessOrEqual;
    else if (operatorString == "<")
        return BinaryOperatorType::Less;
    else if (operatorString == "%")
        return BinaryOperatorType::Modulo;
    else if (operatorString == "&&")
        return BinaryOperatorType::And;
    else if (operatorString == "||")
        return BinaryOperatorType::Or;

    return BinaryOperatorType::Unknown;
}


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
    switch (_binaryOperator)
    {
        case BinaryOperatorType::Add:
            return scope.createManagedObject<IntObject>(left + right);
        case BinaryOperatorType::Minus:
            return scope.createManagedObject<IntObject>(left - right);
        case BinaryOperatorType::Multiply:
            return scope.createManagedObject<IntObject>(left * right);
        case BinaryOperatorType::Divide:
            return scope.createManagedObject<IntObject>(left / right);
        case BinaryOperatorType::Equal:
            return scope.createManagedObject<BoolObject>(left == right);
        case BinaryOperatorType::NotEqual:
            return scope.createManagedObject<BoolObject>(left != right);
        case BinaryOperatorType::GreaterOrEqual:
            return scope.createManagedObject<BoolObject>(left >= right);
        case BinaryOperatorType::Greater:
            return scope.createManagedObject<BoolObject>(left > right);
        case BinaryOperatorType::LessOrEqual:
            return scope.createManagedObject<BoolObject>(left <= right);
        case BinaryOperatorType::Less:
            return scope.createManagedObject<BoolObject>(left < right);
        case BinaryOperatorType::Modulo:
            return scope.createManagedObject<IntObject>(left % right);
        case BinaryOperatorType::And:
            return scope.createManagedObject<BoolObject>(left && right);
        case BinaryOperatorType::Or:
            return scope.createManagedObject<BoolObject>(left || right);
        default:
            ThrowException("cannot apply operator to types Int, Int");
    }
}


BaseObject *BinaryNode::applyOperator(Scope &scope, const FloatObject &left, const FloatObject &right) const
{
    switch (_binaryOperator)
    {
        case BinaryOperatorType::Add:
            return scope.createManagedObject<FloatObject>(left + right);
        case BinaryOperatorType::Minus:
            return scope.createManagedObject<FloatObject>(left - right);
        case BinaryOperatorType::Multiply:
            return scope.createManagedObject<FloatObject>(left * right);
        case BinaryOperatorType::Divide:
            return scope.createManagedObject<FloatObject>(left / right);
        case BinaryOperatorType::Equal:
            return scope.createManagedObject<BoolObject>(left == right);
        case BinaryOperatorType::NotEqual:
            return scope.createManagedObject<BoolObject>(left != right);
        case BinaryOperatorType::GreaterOrEqual:
            return scope.createManagedObject<BoolObject>(left >= right);
        case BinaryOperatorType::Greater:
            return scope.createManagedObject<BoolObject>(left > right);
        case BinaryOperatorType::LessOrEqual:
            return scope.createManagedObject<BoolObject>(left <= right);
        case BinaryOperatorType::Less:
            return scope.createManagedObject<BoolObject>(left < right);
        default:
            ThrowException("cannot apply operator to types Float, Float");
    }
}


BaseObject *BinaryNode::applyOperator(Scope &scope, const StringObject &left, const StringObject &right) const
{
    switch (_binaryOperator)
    {
        case BinaryOperatorType::Add:
            return scope.createManagedObject<StringObject>(left + right);
        case BinaryOperatorType::Equal:
            return scope.createManagedObject<BoolObject>(left == right);
        case BinaryOperatorType::NotEqual:
            return scope.createManagedObject<BoolObject>(left != right);
        default:
            ThrowException("cannot apply operator to types String, String");
    }
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

    ThrowException("cannot apply operator to object types");
}