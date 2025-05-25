/**
 * @file BinaryNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "AnyObject.hpp"
#include "BaseNode.hpp"
#include "Scope.hpp"
#include <string>

enum class BinaryOperatorType : int
{
    Unknown = (-1),
    Add,
    Minus,
    Multiply,
    Divide,
    Equal,
    NotEqual,
    GreaterOrEqual,
    Greater,
    LessOrEqual,
    Less,
    Modulo,
    And,
    Or
};


class BinaryNode : public BaseNode
{
public:
    BinaryNode(BaseNode::Ptr left, BaseNode::Ptr right, const std::string &binaryOperator)
        : _left(left),
          _right(right),
          _binaryOperator(toBinaryOperator(binaryOperator))
    {
        setType(NodeType::Binary);
    }

    AnyObject::Ptr evaluate(Scope &scope) override;

protected:
    AnyObject::Ptr applyOperator(const AnyObject &left, const AnyObject &right) const;

    AnyObject::Ptr applyOperator(bool left, bool right) const;
    AnyObject::Ptr applyOperator(long left, long right) const;
    AnyObject::Ptr applyOperator(double left, double right) const;
    AnyObject::Ptr applyOperator(const std::string &left, const std::string &right) const;
    AnyObject::Ptr applyOperator(const AnyObject::Vector &left, const AnyObject::Vector &right) const;

    /* Convert string to enum (faster if doing lost of comparisons) */
    static BinaryOperatorType toBinaryOperator(const std::string &operatorString);

private:
    BaseNode::Ptr _left{nullptr};
    BaseNode::Ptr _right{nullptr};
    BinaryOperatorType _binaryOperator{BinaryOperatorType::Unknown};
};