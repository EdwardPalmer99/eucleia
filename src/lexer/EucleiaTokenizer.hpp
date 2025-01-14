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
    Tokenizer(const std::string fileString);
    ~Tokenizer() = default;

    /// Create a tokenizer instance from a file path.
    static Tokenizer loadFromFile(const std::string &fpath);

    Token &peek();
    Token next();

    inline bool empty() const
    {
        return _tokens.empty();
    }

protected:
    void skipComment();
    void skipWhitespace();

    // TODO: - should be const methods.
    Token readString();
    Token readNumber();
    Token readID();
    Token readOperator();
    Token readPunctuation();

    void generateTokens();

    Token buildNextToken();

private:
    std::queue<Token> _tokens;
};
