/**
 * @file LookupVariableNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseNode.hpp"
#include <string>

class LookupVariableNode : public BaseNode
{
public:
    LookupVariableNode(std::string variableName_) : variableName(std::move(variableName_)) {}

    // Returns the object in the scope associated with a variable name.
    BaseObject *evaluate(Scope &scope) override;

    std::string variableName;
};