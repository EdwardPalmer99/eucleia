/**
 * @file Token.hpp
 * @author Edward Palmer
 * @date 2025-01-14
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include <ostream>
#include <string>

// TODO: - bang in a namespace.

struct Token
{
    enum TokenType
    {
        None,
        Punctuation,
        Keyword,
        Variable,
        String,
        Operator,
        Int,
        Float,
        Bool
    };

    Token(TokenType _type, std::string &&_value)
        : type{_type}, value{_value}
    {
    }
    Token(TokenType _type, std::string &_value)
        : type{_type}, value{_value}
    {
    }

    static Token blank()
    {
        return Token(None, "");
    }

    std::string description() const;

    TokenType type;
    std::string value;
};


inline std::ostream &operator<<(std::ostream &out, const Token &token)
{
    return (out << std::string("(" + token.description() + ", " + token.value + ")"));
}
