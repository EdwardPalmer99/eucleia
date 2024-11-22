/**
 * @file BinaryNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "AssignNode.hpp"
#include "BaseNode.hpp"
#include "BaseObject.hpp"
#include "FloatObject.hpp"
#include "IntObject.hpp"
#include "Scope.hpp"
#include "StringObject.hpp"
#include <string>

class BinaryNode : public AssignNode
{
public:
    BinaryNode(BaseNode *left_, BaseNode *right_, std::string binaryOperator_)
        : AssignNode(left_, right_),
          binaryOperator(std::move(binaryOperator_))
    {
    }

    BaseObject *evaluate(Scope &scope) override;

    std::string binaryOperator;


    BaseObject *applyOperator(Scope &scope, const BaseObject &left, const BaseObject &right) const;

    BaseObject *applyOperator(Scope &scope, const IntObject &left, const IntObject &right) const;
    BaseObject *applyOperator(Scope &scope, const FloatObject &left, const FloatObject &right) const;
    BaseObject *applyOperator(Scope &scope, const StringObject &left, const StringObject &right) const;
};