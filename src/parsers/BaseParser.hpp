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

    BaseParser() = delete;

    /* Prevent direct initialization */
    explicit BaseParser(Tokens &tokens_) : _tokens(tokens_) {}

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

    /* Returns a reference to the tokens */
    [[nodiscard]] Tokens &tokens() { return _tokens; }

private:
    /* Stores a reference to the parsed file tokens */
    Tokens &_tokens;
};
