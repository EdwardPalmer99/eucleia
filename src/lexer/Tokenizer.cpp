/**
 * @file Tokenizer.cpp
 * @author Edward Palmer
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Tokenizer.hpp"
#include "Exceptions.hpp"
#include "Grammar.hpp"
#include "Logger.hpp"
#include "Stringify.hpp"
#include <iostream>


Tokenizer::Tokenizer(const std::string &fpath)
    : stream(fpath)
{
    buildTokens();
}


void Tokenizer::buildTokens()
{
    while (!stream.isLast())
    {
        Token token = buildNextToken();
        Logger::debug(stream.location() + ": " + token.print());

        if (!token.type == Token::EndOfFile)
            tokens.push(std::move(token));
    }
}


const Token &Tokenizer::peek() const
{
    if (tokens.empty())
        ThrowException("cannot peek(). No tokens remaining");

    return tokens.front();
}


Token Tokenizer::next()
{
    Token next = peek();

    if (!tokens.empty())
        tokens.pop();

    return next;
}


bool Tokenizer::empty() const
{
    return tokens.empty();
}


Token Tokenizer::buildNextToken()
{
    if (stream.isLast())
    {
        return Token(Token::EndOfFile, "");
    }
    else if (stream.isComment() || stream.isShebang())
    {
        skipLine();
        return buildNextToken();
    }
    else if (stream.isQuote())
    {
        return buildStringToken();
    }
    else if (stream.isDigit())
    {
        return buildNumberToken();
    }
    else if (stream.isWhiteSpace())
    {
        (void)stream.increment();
        return buildNextToken();
    }
    else if (stream.isPunctuation())
    {
        return buildPunctuationToken();
    }
    else if (stream.isOperator())
    {
        return buildOperatorToken();
    }
    else if (stream.isID())
    {
        return buildIDToken();
    }

    ThrowException(stream.location() + eucleia::stringify(": failed to process character: %c", stream.current()));
}


void Tokenizer::skipLine()
{
    do
    {
        (void)stream.increment();
    } while (!stream.isLast() && !stream.isNewLine());
}


Token Tokenizer::buildStringToken()
{
    if (!stream.isQuote())
    {
        ThrowException(stream.location() + ": missing opening quote for string");
    }

    // Allocate:
    size_t capacity = 100;
    size_t count = 0;

    char *value = (char *)malloc(sizeof(char) * capacity);
    if (!value)
    {
        ThrowException(stream.location() + ": failed to malloc array");
    }

    bool foundEndQuote = false;

    while (!stream.isLast())
    {
        (void)stream.increment();

        if (stream.isQuote())
        {
            foundEndQuote = true;
            break;
        }

        value[count++] = stream.current();

        if (count >= capacity)
        {
            capacity <<= 2; // Double capacity.

            void *tempPtr = realloc(value, sizeof(char) * capacity);
            if (!tempPtr) // Failed to resize array.
            {
                free(value); // Free old block.
                ThrowException(stream.location() + ": failed to resize array");
            }

            value = (char *)tempPtr; // Resized array.
        }
    }

    if (!stream.isQuote()) // Should be the endquote.
    {
        free(value); // Cleanup memory.
        ThrowException(stream.location() + ": missing end quote");
    }

    // Skip end-quote.
    (void)stream.increment();

    // Terminate string:
    value[count++] = '\0';

    // Resize to fit:
    if (count < capacity)
    {
        value = (char *)realloc(value, sizeof(char) * count);
    }

    // Create string token:
    Token token = Token(Token::String, std::string(value));

    // Handle memory:
    free(value);

    return token;
}


Token Tokenizer::buildNumberToken()
{
    if (!stream.isDigit())
    {
        ThrowException(stream.location() + " : expected digit");
    }

    bool readDecimal = false;

    int count = 0;
    char value[100];

    while (stream.isDigit() || (stream.current() == '.'))
    {
        if (stream.current() == '.')
        {
            if (readDecimal == true) // Oh dear! We've seen '.' twice now.
            {
                ThrowException(stream.location() + ": seen multiple '.' characters in number");
            }

            readDecimal = true;
        }

        value[count++] = stream.current();

        (void)stream.increment();
    }

    value[count] = '\0'; // Terminate.

    return Token(readDecimal ? Token::Float : Token::Int, std::string(value));
}


Token Tokenizer::buildIDToken()
{
    if (!stream.isID())
    {
        ThrowException(stream.location() + " : unexpected ID character");
    }

    std::vector<char> buffer;

    while (stream.isID())
    {
        buffer.push_back(stream.current());
        (void)stream.increment();
    }

    buffer.push_back('\0');

    std::string stringID(buffer.data());

    return Token(Grammar::isKeyword(stringID) ? Token::Keyword : Token::Variable, stringID);
}


Token Tokenizer::buildPunctuationToken()
{
    if (!stream.isPunctuation())
    {
        ThrowException(stream.location() + ": expected punctuation");
    }

    char buffer[2] = {stream.current(), '\0'};
    (void)stream.increment();

    return Token(Token::Punctuation, std::string(buffer));
}


Token Tokenizer::buildOperatorToken()
{
    if (!stream.isOperator())
    {
        ThrowException(stream.location() + ": expected operator");
    }

    std::vector<char> buffer; // Will allow other operators in future (ie +=).

    while (stream.isOperator())
    {
        buffer.push_back(stream.current());
        (void)stream.increment();
    }

    buffer.push_back('\0');

    return Token(Token::Operator, std::string(buffer.data()));
}
