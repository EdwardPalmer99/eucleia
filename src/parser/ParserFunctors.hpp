/**
 * @file ParserFunctors.hpp
 * @author Edward Palmer
 * @date 2025-04-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "Exceptions.hpp"
#include "Token.hpp"

/* Functor for skipping tokens */
class SkipFunctor
{
public:
    SkipFunctor(Tokens &tokens) : _tokens(tokens) {}

    /* Skips a token */
    void operator()(std::string expected)
    {
        const Token &token = _tokens.front();

        if (token != expected)
        {
            ThrowException("unexpected token: '" + token + "' != '" + expected + "'");
        }

        _tokens.pop();
    }

protected:
    /* Maintain a reference to tokens */
    Tokens &_tokens;
};
