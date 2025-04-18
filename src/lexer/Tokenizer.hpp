/**
 * @file Tokens.hpp
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

/* Stores all tokens */
class Tokens : private std::queue<Token>
{
public:
    /* Pops and returns the token at the front of the queue */
    [[nodiscard]] Token dequeue();

    /* Returns the front token */
    using std::queue<Token>::front;

    /* Skip token */
    using std::queue<Token>::pop;

    /* empty() */
    using std::queue<Token>::empty;

protected:
    friend class Tokenizer;

    using std::queue<Token>::push;
};


/* Constructs Tokens from a file */
class Tokenizer
{
public:
    static Tokens build(const std::string &path)
    {
        return Tokenizer().buildTokens(path);
    }

protected:
    /* Prevent direct initialization */
    Tokenizer() = default;

    /* Construct tokens from a file */
    Tokens buildTokens(const std::string &path);

    void skipLine(CharStream &stream);

    Token buildNextToken(CharStream &stream);

    Token buildStringToken(CharStream &stream);
    Token buildNumberToken(CharStream &stream);
    Token buildIDToken(CharStream &stream);
    Token buildOperatorToken(CharStream &stream);
    Token buildPunctuationToken(CharStream &stream);
};
