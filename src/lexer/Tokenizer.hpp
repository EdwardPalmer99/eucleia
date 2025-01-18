/**
 * @file Tokenizer.hpp
 * @author Edward Palmer
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "CharStream.hpp"
#include "Token.hpp"
#include <queue>
#include <string>

class Tokenizer
{
public:
    Tokenizer() = delete;
    Tokenizer(const std::string &path);

    const Token &peek() const;

    Token next();

    bool empty() const;

private:
    Token buildNextToken();

    void buildTokens();
    void skipLine();

    Token buildStringToken();
    Token buildNumberToken();
    Token buildIDToken();
    Token buildOperatorToken();
    Token buildPunctuationToken();

    CharStream stream;
    std::queue<Token> tokens;
};
