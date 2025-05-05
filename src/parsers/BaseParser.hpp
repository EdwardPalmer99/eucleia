/**
 * @file BaseParser.hpp
 * @author Edward Palmer
 * @date 2025-04-21
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "BaseNode.hpp"
#include "ProgramNode.hpp"
#include "Tokenizer.hpp"
#include <functional>
#include <string>

class BaseParser
{
protected:
    /* Aliases */
    using ParseMethod = std::function<BaseNode *(void)>;

    /* Prevent direct initialization */
    BaseParser() = default;
    BaseParser(Tokens &&tokens);

    /* Parse a delimited expression, e.g. (a, b, c) where start='(', stop=')', separator=',' */
    ProgramNode *parseDelimited(std::string start,
                                std::string stop,
                                std::string separator,
                                ParseMethod expression);

    /* Returns true if front token matches expected value */
    bool equals(const std::string &value);

    /* Returns true if front token matches expected type */
    bool equals(Token::Type type);

    /* Returns true if front token matches exepcted value and type */
    bool equals(Token::Type type, const std::string &value);

    /* Skips token and throws exception if token to be skipped does not match expected */
    void skip(const std::string &expected);

    /* Stores parsed file tokens */
    Tokens _tokens;
};
