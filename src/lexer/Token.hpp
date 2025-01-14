/**
 * @file Token.hpp
 * @author Edward Palmer
 * @date 2025-01-14
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include <string>

class Token
{
public:
    enum TokenType
    {
        EndOfFile,
        Punctuation,
        Keyword,
        Variable,
        String,
        Operator,
        Int,
        Float
    };

    Token() = delete;
    Token(TokenType type, std::string value);

    std::string print() const;

    const TokenType type;
    const std::string value;

private:
    std::string typeName() const;
};
