/**
 * @file BinaryNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseNode.hpp"
#include "BaseObject.hpp"
#include "FloatObject.hpp"
#include "IntObject.hpp"
#include "Scope.hpp"
#include "StringObject.hpp"
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

    BaseObject::Ptr evaluate(Scope &scope) override;

protected:
    BaseObject::Ptr applyOperator(const BaseObject &left, const BaseObject &right) const;

    BaseObject::Ptr applyOperator(const IntObject &left, const IntObject &right) const;
    BaseObject::Ptr applyOperator(const FloatObject &left, const FloatObject &right) const;
    BaseObject::Ptr applyOperator(const StringObject &left, const StringObject &right) const;

    /* Convert string to enum (faster if doing lost of comparisons) */
    static BinaryOperatorType toBinaryOperator(const std::string &operatorString);

private:
    BaseNode::Ptr _left{nullptr};
    BaseNode::Ptr _right{nullptr};
    BinaryOperatorType _binaryOperator{BinaryOperatorType::Unknown};
};