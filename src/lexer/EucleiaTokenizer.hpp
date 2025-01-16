//
//  EucleiaTokenizer.hpp
//  Eucleia
//
//  Created by Edward on 18/01/2024.
//

#pragma once
#include "EucleiaInputStream.hpp"
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
