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

BinaryOperatorType binaryOperatorType(const std::string &operatorString);


class BinaryNode : public BaseNode
{
public:
    BinaryNode(BaseNode::Ptr left_, BaseNode::Ptr right_, BinaryOperatorType binaryOperator)
        : BaseNode(NodeType::Binary), _left(left_), _right(right_),
          _binaryOperator(binaryOperator)
    {
        setType(NodeType::Binary);
    }

    ~BinaryNode() override = default;

    BaseObject::Ptr evaluate(Scope &scope) override;

protected:
    BaseObject::Ptr applyOperator(const BaseObject &left, const BaseObject &right) const;

    BaseObject::Ptr applyOperator(const IntObject &left, const IntObject &right) const;
    BaseObject::Ptr applyOperator(const FloatObject &left, const FloatObject &right) const;
    BaseObject::Ptr applyOperator(const StringObject &left, const StringObject &right) const;

private:
    BaseNode::Ptr _left{nullptr};
    BaseNode::Ptr _right{nullptr};
    BinaryOperatorType _binaryOperator{BinaryOperatorType::Unknown};
};