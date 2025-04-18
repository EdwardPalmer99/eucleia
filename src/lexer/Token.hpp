/**
 * @file Token.hpp
 * @author Edward Palmer
 * @date 2025-01-14
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include <string>

/* Stores the tokens read from the user's file */
class Token : public std::string
{
public:
    enum Type
    {
        NotSet = (-1),
        EndOfFile,
        Punctuation,
        Keyword,
        Variable,
        String,
        Operator,
        Int,
        Float
    };

    /* Constructors */
    Token(Type type) : _type(type) {}
    Token(std::string &value, Type type = NotSet) : std::string(value), _type(type) {}
    Token(std::string &&value = "", Type type = NotSet) : std::string(value), _type(type) {}

    /* Reference to type */
    [[nodiscard]] inline Type &type()
    {
        return _type;
    }

    [[nodiscard]] inline Type type() const
    {
        return _type;
    }

private:
    /* Type of the token */
    Type _type{Type::NotSet};
};
