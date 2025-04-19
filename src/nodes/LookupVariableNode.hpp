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
#include "Token.hpp"
#include <string>

class FileParser;

class LookupVariableNode : public BaseNode
{
public:
    LookupVariableNode(std::string name_) : name(std::move(name_)) {}
    LookupVariableNode(Token token) : name(token.validated(Token::Variable)) {}

    static LookupVariableNode *parse(FileParser &parser);

    // Returns the object in the scope associated with a variable name.
    BaseObject *evaluate(Scope &scope) override;

    std::string name;
};