/**
 * @file Tokenizer.hpp
 * @author Edward Palmer
 * @date 2025-01-18
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once
#include "InputStream.hpp"
#include "Token.hpp"
#include <queue>
#include <set>
#include <string>

class Tokenizer : public InputStream
{
public:
    Tokenizer() = delete;
    Tokenizer(const std::string &path);

    const Token &peek() const;

    Token next();

    bool empty() const;

protected:
    void generateTokens();

    void skipComment();
    void skipWhitespace();

    Token readString();
    Token readNumber();
    Token readID();
    Token readOperator();
    Token readPunctuation();

    Token buildNextToken();

private:
    std::queue<Token> tokens;
};
