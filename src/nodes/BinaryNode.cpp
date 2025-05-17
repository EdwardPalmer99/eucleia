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
#include "ObjectFactory.hpp"
#include "Objects.hpp"
#include <sstream>


BinaryOperatorType BinaryNode::toBinaryOperator(const std::string &operatorString)
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


BaseObject::Ptr BinaryNode::evaluate(Scope &scope)
{
    auto leftEvaluated = _left->evaluate(scope);
    auto rightEvaluated = _right->evaluate(scope);

    // Persist result by storing in outer scope.
    return applyOperator(*leftEvaluated, *rightEvaluated);
}


BaseObject::Ptr BinaryNode::applyOperator(const IntObject &left, const IntObject &right) const
{
    switch (_binaryOperator)
    {
        case BinaryOperatorType::Add:
            return ObjectFactory::allocate<IntObject>(left + right);
        case BinaryOperatorType::Minus:
            return ObjectFactory::allocate<IntObject>(left - right);
        case BinaryOperatorType::Multiply:
            return ObjectFactory::allocate<IntObject>(left * right);
        case BinaryOperatorType::Divide:
            return ObjectFactory::allocate<IntObject>(left / right);
        case BinaryOperatorType::Equal:
            return ObjectFactory::allocate<BoolObject>(left == right);
        case BinaryOperatorType::NotEqual:
            return ObjectFactory::allocate<BoolObject>(left != right);
        case BinaryOperatorType::GreaterOrEqual:
            return ObjectFactory::allocate<BoolObject>(left >= right);
        case BinaryOperatorType::Greater:
            return ObjectFactory::allocate<BoolObject>(left > right);
        case BinaryOperatorType::LessOrEqual:
            return ObjectFactory::allocate<BoolObject>(left <= right);
        case BinaryOperatorType::Less:
            return ObjectFactory::allocate<BoolObject>(left < right);
        case BinaryOperatorType::Modulo:
            return ObjectFactory::allocate<IntObject>(left % right);
        case BinaryOperatorType::And:
            return ObjectFactory::allocate<BoolObject>(left && right);
        case BinaryOperatorType::Or:
            return ObjectFactory::allocate<BoolObject>(left || right);
        default:
            ThrowException("cannot apply operator to types Int, Int");
    }
}


BaseObject::Ptr BinaryNode::applyOperator(const FloatObject &left, const FloatObject &right) const
{
    switch (_binaryOperator)
    {
        case BinaryOperatorType::Add:
            return ObjectFactory::allocate<FloatObject>(left + right);
        case BinaryOperatorType::Minus:
            return ObjectFactory::allocate<FloatObject>(left - right);
        case BinaryOperatorType::Multiply:
            return ObjectFactory::allocate<FloatObject>(left * right);
        case BinaryOperatorType::Divide:
            return ObjectFactory::allocate<FloatObject>(left / right);
        case BinaryOperatorType::Equal:
            return ObjectFactory::allocate<BoolObject>(left == right);
        case BinaryOperatorType::NotEqual:
            return ObjectFactory::allocate<BoolObject>(left != right);
        case BinaryOperatorType::GreaterOrEqual:
            return ObjectFactory::allocate<BoolObject>(left >= right);
        case BinaryOperatorType::Greater:
            return ObjectFactory::allocate<BoolObject>(left > right);
        case BinaryOperatorType::LessOrEqual:
            return ObjectFactory::allocate<BoolObject>(left <= right);
        case BinaryOperatorType::Less:
            return ObjectFactory::allocate<BoolObject>(left < right);
        default:
            ThrowException("cannot apply operator to types Float, Float");
    }
}


BaseObject::Ptr BinaryNode::applyOperator(const StringObject &left, const StringObject &right) const
{
    switch (_binaryOperator)
    {
        case BinaryOperatorType::Add:
            return ObjectFactory::allocate<StringObject>(left + right);
        case BinaryOperatorType::Equal:
            return ObjectFactory::allocate<BoolObject>(left == right);
        case BinaryOperatorType::NotEqual:
            return ObjectFactory::allocate<BoolObject>(left != right);
        default:
            ThrowException("cannot apply operator to types String, String");
    }
}


BaseObject::Ptr BinaryNode::applyOperator(const BaseObject &left, const BaseObject &right) const
{
    // TODO: - implement + operator for other object types. Will mean we don't need any if/else statements.

    if (left.isObjectType<IntObject>() && right.isObjectType<IntObject>())
    {
        return applyOperator(left.castObject<IntObject>(), right.castObject<IntObject>());
    }
    else if (left.isObjectType<FloatObject>() && right.isObjectType<FloatObject>())
    {
        return applyOperator(left.castObject<FloatObject>(), right.castObject<FloatObject>());
    }
    else if (left.isObjectType<IntObject>() && right.isObjectType<FloatObject>())
    {
        return applyOperator(left.castObject<IntObject>().castToFloat(), right.castObject<FloatObject>());
    }
    else if (left.isObjectType<FloatObject>() && right.isObjectType<IntObject>())
    {
        return applyOperator(left.castObject<FloatObject>(), right.castObject<IntObject>().castToFloat());
    }
    else if (left.isObjectType<StringObject>() && right.isObjectType<StringObject>())
    {
        return applyOperator(left.castObject<StringObject>(), right.castObject<StringObject>());
    }
    else if (left.isObjectType<ArrayObject>() && right.isObjectType<ArrayObject>())
    {
        return ObjectFactory::allocate<ArrayObject>(left.castObject<ArrayObject>() + right.castObject<ArrayObject>());
    }

    std::stringstream oss;
    oss << "cannot apply operator [" << int(_binaryOperator) << "] to objects [" << left << "] and [" << right << "]";

    ThrowException(oss.str());
}
