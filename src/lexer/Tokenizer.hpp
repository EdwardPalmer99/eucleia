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
#include <string>


/* Builder class for Tokens */
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
