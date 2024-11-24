//
//  EucleiaTokenizer.cpp
//  Eucleia
//
//  Created by Edward on 18/01/2024.
//

#include "EucleiaTokenizer.hpp"
#include "EucleiaFileReader.hpp"
#include "EucleiaUtility.hpp"
#include <iostream>

Tokenizer Tokenizer::loadFromFile(const std::string &fpath)
{
    using namespace eucleia;

    EucleiaFileReader fileReader;

    return Tokenizer(fileReader.readFile(fpath));
}


std::string Token::description() const
{
    switch (type)
    {
        case None:
            return "None";
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
        case Bool:
            return "Bool";
        default:
            return "Unknown";
    }
}


Tokenizer::Tokenizer(const std::string fileString)
    : InputStream(std::move(fileString))
{
    generateTokens();
}


void Tokenizer::generateTokens()
{
    while (!InputStream::isEof())
    {
        auto token = buildNextToken();

        if (token.type != Token::None)
        {
            // std::cout << token << std::endl;
            _tokens.push(std::move(token));
        }
    }
}


Token &Tokenizer::peek()
{
    return _tokens.front();
}


Token Tokenizer::next()
{
    auto next = Tokenizer::peek();
    _tokens.pop();

    return next;
}

bool Tokenizer::isDataTypeToken()
{
    return grammar().isDataType(peek().value);
}


Token Tokenizer::buildNextToken()
{
    char c = InputStream::peek();

    if (isComment())
    {
        skipComment();
        return buildNextToken();
    }
    else if (isStringStart())
    {
        return readString();
    }
    else if (isDigit())
    {
        return readNumber();
    }
    else if (isWhiteSpace())
    {
        skipWhitespace();
        return buildNextToken();
    }
    else if (isPunctuation())
    {
        return readPunctuation();
    }
    else if (isOperator())
    {
        return readOperator();
    }
    else if (isID())
    {
        return readID();
    }
    else if (isEof())
    {
        return Token::blank();
    }
    else
    {
        reportError("Failed to process character '%c'\n", c);
        exit(EXIT_FAILURE);
    }
}


void Tokenizer::skipComment()
{
    do
    {
        InputStream::next();
    } while (!isEof() && !isNewLine());
}


void Tokenizer::skipWhitespace(void)
{
    InputStream::next();
}


#pragma mark -

Token Tokenizer::readString()
{
    size_t capacity = 100;
    size_t count = 0;

    char *value = (char *)malloc(sizeof(char) * capacity);
    if (!value)
    {
        exit(EXIT_FAILURE);
    }

    char c = InputStream::next();

    do
    {
        if (c != '"')
        {
            value[count++] = c;
        }

        if (count >= capacity)
        {
            capacity <<= 2; // Double capacity.

            void *tempPtr = realloc(value, sizeof(char) * capacity);
            if (!tempPtr) // Failed to resize array.
            {
                EucleiaError("%s", "failed to resize array.");
            }

            value = (char *)tempPtr; // Resized array.
        }

        c = InputStream::next();
    } while (!(isEof() || c == '"'));

    // Terminate value string.
    value[count++] = '\0';

    // Resize to fit.
    if (count < capacity)
    {
        value = (char *)realloc(value, sizeof(char) * count);
    }

    Token token = Token(Token::String, std::string(value));

    // Handle memory.
    free(value);

    return token;
}


Token Tokenizer::readNumber()
{
    bool readDecimal = false;

    int count = 0;
    char value[100];

    while (isdigit(InputStream::peek()) || InputStream::peek() == '.')
    {
        char c = InputStream::next();

        if (c == '.')
        {
            if (readDecimal == true) // Oh dear! We've seen '.' twice now.
            {
                reportError("Seen multiple '.' characters in number.");
            }

            readDecimal = true;
        }

        value[count++] = c;
    }

    value[count] = '\0'; // Terminate string.

    return Token(readDecimal ? Token::Float : Token::Int, std::string(value));
}


Token Tokenizer::readID()
{
    std::vector<char> buffer;

    while (isID())
    {
        buffer.push_back(InputStream::next());
    }

    buffer.push_back('\0');

    std::string stringID(buffer.data());

    return Token(isKeyword(stringID) ? Token::Keyword : Token::Variable, stringID);
}


Token Tokenizer::readPunctuation()
{
    char c = InputStream::next();

    char buffer[2] = {c, '\0'};

    return Token(Token::Punctuation, std::string(buffer));
}


Token Tokenizer::readOperator()
{
    std::vector<char> buffer; // Will allow other operators in future (ie +=).

    while (isOperator())
    {
        buffer.push_back(InputStream::next());
    }

    buffer.push_back('\0');

    return Token(Token::Operator, std::string(buffer.data()));
}


#pragma mark -

bool Tokenizer::isKeyword(const std::string &possibleKeyword) const
{
    return grammar().isKeyword(possibleKeyword);
}
