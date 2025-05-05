/**
 * @file BaseParser.cpp
 * @author Edward Palmer
 * @date 2025-04-21
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "BaseParser.hpp"
#include "Exceptions.hpp"


ProgramNode *BaseParser::parseDelimited(std::string start,
                                        std::string stop,
                                        std::string separator,
                                        ParseMethod parseMethod)
{
    skip(start); // Skip the punctuation at the start.

    std::vector<BaseNode *> parsedNodes;

    // Iterate while we still have tokens and haven't reached stop token.
    bool firstCall = true;

    while (!_tokens.empty() && !equals(stop))
    {
        // Skip separator on each subsequent call (i.e. a, b)
        if (firstCall)
        {
            firstCall = false;
        }
        else
        {
            skip(separator);
        }

        if (equals(stop))
        {
            break;
        }

        // Parse the token.
        parsedNodes.push_back(parseMethod());
    }

    skip(stop); // Skip the punctuation at the end.

    // Resize to fit.
    parsedNodes.shrink_to_fit();

    return new ProgramNode(parsedNodes);
}


bool BaseParser::equals(const std::string &value)
{
    return (_tokens.empty() ? false : (_tokens.front() == value));
}


bool BaseParser::equals(Token::Type type)
{
    return (_tokens.empty() ? false : (_tokens.front().type() == type));
}


bool BaseParser::equals(Token::Type type, const std::string &value)
{
    return (equals(type) && equals(value));
}


void BaseParser::skip(const std::string &expected)
{
    if (!equals(expected))
    {
        /* TODO: - add method wrapper so we never have exception if calling front() on empty queue */
        ThrowException("unexpected token: " + _tokens.front() + "!=" + expected);
    }

    _tokens.pop();
}
