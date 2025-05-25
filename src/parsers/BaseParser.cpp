/**
 * @file BaseParser.cpp
 * @author Edward Palmer
 * @date 2025-04-21
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "BaseParser.hpp"
#include "AnyObject.hpp"
#include "Exceptions.hpp"


bool BaseParser::equals(const std::string &value)
{
    return (tokens().empty() ? false : (tokens().front() == value));
}


bool BaseParser::equals(Token::Type type)
{
    return (tokens().empty() ? false : (tokens().front().type() == type));
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
        ThrowException("unexpected token: '" + tokens().front() + "' != '" + expected + "'");
    }

    tokens().pop();
}
