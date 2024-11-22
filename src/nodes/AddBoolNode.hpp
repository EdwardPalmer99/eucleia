/**
 * @file AddBoolNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseNode.hpp"
#include "EucleiaScope.hpp"
#include "IntObject.hpp"


class AddBoolNode : public BaseNode
{
public:
    AddBoolNode(bool state_) : boolObject(state_) {}

    // Creates a BoolObject in the scope and returns managed pointer to it.
    BoolObject *evaluate(Scope &scope) override;


    BoolObject boolObject;
};