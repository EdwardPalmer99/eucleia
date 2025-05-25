/**
 * @file BinaryNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "BinaryNode.hpp"
#include "Exceptions.hpp"
#include "ObjectFactory.hpp"
#include <sstream>


BinaryOperatorType BinaryNode::toBinaryOperator(const std::string &operatorString)
{
    // TODO: - make into static hashtable for speeding up
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


AnyObject BinaryNode::evaluate(Scope &scope)
{
    auto leftEvaluated = _left->evaluate(scope);
    auto rightEvaluated = _right->evaluate(scope);

    // Persist result by storing in outer scope.
    return applyOperator(leftEvaluated, rightEvaluated);
}


AnyObject BinaryNode::applyOperator(const AnyObject &left, const AnyObject &right) const
{
    if (left.isType(AnyObject::Bool) && right.isType(AnyObject::Bool))
    {
        return applyOperator(left.getValue<bool>(), right.getValue<bool>());
    }
    if (left.isType(AnyObject::Int) && right.isType(AnyObject::Int))
    {
        return applyOperator(left.getValue<long>(), right.getValue<long>());
    }
    else if (left.isType(AnyObject::Float) && right.isType(AnyObject::Float)) /* Implicit casts */
    {
        return applyOperator(left.getValue<double>(), right.getValue<double>());
    }
    else if (left.isType(AnyObject::Int) && right.isType(AnyObject::Float))
    {
        return applyOperator((double)left.getValue<long>(), right.getValue<double>());
    }
    else if (left.isType(AnyObject::Float) && right.isType(AnyObject::Int))
    {
        return applyOperator(left.getValue<double>(), (double)right.getValue<long>());
    }
    else if (left.isType(AnyObject::String) && right.isType(AnyObject::String))
    {
        return applyOperator(left.getValue<std::string>(), right.getValue<std::string>());
    }
    else if (left.isType(AnyObject::Array) && right.isType(AnyObject::Array))
    {
        return applyOperator(left.getValue<AnyObject::Vector>(), right.getValue<AnyObject::Vector>());
    }

    std::stringstream oss;
    oss << "cannot apply operator [" << int(_binaryOperator) << "] to objects [" << left << "] and [" << right << "]";

    ThrowException(oss.str());
}


AnyObject BinaryNode::applyOperator(const AnyObject::Vector &left, const AnyObject::Vector &right) const
{
    switch (_binaryOperator) // TODO: - does this work? Test this code
    {
        case BinaryOperatorType::Add:
        {
            // TODO: - this will probably require us to copy the vector otherwise we're doing shallow copying
            AnyObject::Vector result;

            result.reserve(left.size() + right.size());
            result.insert(result.end(), left.begin(), left.end());
            result.insert(result.end(), right.begin(), right.end());

            return AnyObject(std::move(result));
        }
        default:
            ThrowException("cannot apply operator to types Array, Array");
    }
}


AnyObject BinaryNode::applyOperator(bool left, bool right) const
{
    switch (_binaryOperator)
    {
        case BinaryOperatorType::Equal:
            return AnyObject(left == right);
        case BinaryOperatorType::NotEqual:
            return AnyObject(left != right);
        case BinaryOperatorType::And:
            return AnyObject(left && right);
        case BinaryOperatorType::Or:
            return AnyObject(left != right);
        default:
            ThrowException("cannot apply operator to types Bool, Bool");
    }
}


AnyObject BinaryNode::applyOperator(long left, long right) const
{
    switch (_binaryOperator)
    {
        case BinaryOperatorType::Add:
            return AnyObject(left + right);
        case BinaryOperatorType::Minus:
            return AnyObject(left - right);
        case BinaryOperatorType::Multiply:
            return AnyObject(left * right);
        case BinaryOperatorType::Divide:
            return AnyObject(left / right);
        case BinaryOperatorType::Equal:
            return AnyObject(left == right);
        case BinaryOperatorType::NotEqual:
            return AnyObject(left != right);
        case BinaryOperatorType::GreaterOrEqual:
            return AnyObject(left >= right);
        case BinaryOperatorType::Greater:
            return AnyObject(left > right);
        case BinaryOperatorType::LessOrEqual:
            return AnyObject(left <= right);
        case BinaryOperatorType::Less:
            return AnyObject(left < right);
        case BinaryOperatorType::Modulo:
            return AnyObject(left % right);
        case BinaryOperatorType::And:
            return AnyObject(left && right);
        case BinaryOperatorType::Or:
            return AnyObject(left || right);
        default:
            ThrowException("cannot apply operator to types Int, Int");
    }
}


AnyObject BinaryNode::applyOperator(double left, double right) const
{
    switch (_binaryOperator)
    {
        case BinaryOperatorType::Add:
            return AnyObject(left + right);
        case BinaryOperatorType::Minus:
            return AnyObject(left - right);
        case BinaryOperatorType::Multiply:
            return AnyObject(left * right);
        case BinaryOperatorType::Divide:
            return AnyObject(left / right);
        case BinaryOperatorType::Equal:
            return AnyObject(left == right);
        case BinaryOperatorType::NotEqual:
            return AnyObject(left != right);
        case BinaryOperatorType::GreaterOrEqual:
            return AnyObject(left >= right);
        case BinaryOperatorType::Greater:
            return AnyObject(left > right);
        case BinaryOperatorType::LessOrEqual:
            return AnyObject(left <= right);
        case BinaryOperatorType::Less:
            return AnyObject(left < right);
        default:
            ThrowException("cannot apply operator to types Float, Float");
    }
}


AnyObject BinaryNode::applyOperator(const std::string &left, const std::string &right) const
{
    switch (_binaryOperator)
    {
        case BinaryOperatorType::Add:
            return AnyObject(left + right);
        case BinaryOperatorType::Equal:
            return AnyObject(left == right);
        case BinaryOperatorType::NotEqual:
            return AnyObject(left != right);
        default:
            ThrowException("cannot apply operator to types String, String");
    }
}
