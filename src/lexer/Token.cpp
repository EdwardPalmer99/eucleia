/**
 * @file Token.cpp
 * @author Edward Palmer
 * @date 2025-05-24
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Token.hpp"


std::string Token::typeToString() const /* TODO: - more efficient to have a static maps and return a reference to string */
{
    switch (_type)
    {
        case NotSet:
            return "NotSet";
        case EndOfFile:
            return "EndOfFile";
        case Punctuation:
            return "Punctuation";
        case Keyword:
            return "Keyword";
        case Variable:
            return "Variable";
        case String:
            return "String";
        case Operator:
            return "Operator";
        case Int:
            return "Int";
        case Float:
            return "Float";
        default:
            ThrowException("Unknown token type");
    }
}