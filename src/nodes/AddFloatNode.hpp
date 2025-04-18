/**
 * @file AddFloatNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseNode.hpp"
#include "FloatObject.hpp"
#include "Token.hpp"

class AddFloatNode : public BaseNode
{
public:
    AddFloatNode(double float_) : floatObject(float_) {}
    AddFloatNode(Token token) : AddFloatNode(token.toFloat()) {}

    // Returns a FloatObject in the current scope and returns a managed pointer.
    FloatObject *evaluate(Scope &scope) override;

    FloatObject floatObject;
};