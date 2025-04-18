/**
 * @file Token.hpp
 * @author Edward Palmer
 * @date 2025-01-14
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "Exceptions.hpp"
#include <queue>
#include <string>


/* Stores a token */
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


/* Stores all tokens */
class Tokens : private std::queue<Token>
{
public:
    /* Pops and returns the token at the front of the queue */
    [[nodiscard]] inline Token dequeue();

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


Token Tokens::dequeue()
{
    if (empty())
    {
        ThrowException("Token queue is empty!");
    }

    Token next = front();
    pop();

    return next;
}