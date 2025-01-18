/**
 * @file Token.cpp
 * @author Edward Palmer
 * @date 2025-01-14
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Token.hpp"
#include "Exceptions.hpp"

Token::Token(TokenType type_, std::string value_)
    : type{type_}, value{std::move(value_)}
{
}


std::string Token::print() const
{
    return typeName() + ": " + value;
}


std::string Token::typeName() const
{
    switch (type)
    {
        case EndOfFile:
            return "end-of-file";
        case Punctuation:
            return "punctuation";
        case Keyword:
            return "keyword";
        case Variable:
            return "variable";
        case String:
            return "string";
        case Operator:
            return "other";
        case Int:
            return "int";
        case Float:
            return "float";
        default:
            ThrowException("unexpected token type: " + std::to_string(type));
    }
}