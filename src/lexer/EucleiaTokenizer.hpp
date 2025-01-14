//
//  EucleiaTokenizer.hpp
//  Eucleia
//
//  Created by Edward on 18/01/2024.
//

#pragma once
#include "EucleiaInputStream.hpp"
#include <queue>
#include <set>
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
