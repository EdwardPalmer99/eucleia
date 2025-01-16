/**
 * @file EucleiaInputStream.cpp
 * @author Edward Palmer
 * @date 2025-01-16
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "EucleiaInputStream.hpp"
#include "EucleiaFileReader.hpp"
#include "Exceptions.hpp"
#include "Grammar.hpp"
#include "Stringify.hpp"
#include <algorithm>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

InputStream::InputStream(const std::string &path)
{
    // Load file from path and set pointer.
    fileContents = eucleia::loadFileContents(path.c_str());
    current.ptr = fileContents;
}


InputStream::~InputStream()
{
    if (fileContents != nullptr)
        free(fileContents);
}

char InputStream::peek() const
{
    return (current.ptr ? *current.ptr : '\0');
}

char *InputStream::peek2()
{
    static char buffer[3];
    memset(buffer, '\0', 3);

    buffer[0] = peek();

    if (buffer[0] != '\0')
    {
        current.ptr++;
        buffer[1] = peek();
        current.ptr--;
    }

    return buffer;
}

char InputStream::next()
{
    char c = peek();

    consume();

    return c;
}

bool InputStream::isEof() const
{
    return (peek() == '\0');
}

bool InputStream::isNewLine() const
{
    return (peek() == '\n');
}

bool InputStream::isStringStart() const
{
    return (peek() == '"');
}

bool InputStream::isComment()
{
    return (strcmp(peek2(), "//") == 0);
}
bool InputStream::isOperator() const
{
    return Grammar::isOperator(peek());
}

bool InputStream::isPunctuation() const
{
    return Grammar::isPunctuation(peek());
}

bool InputStream::isDigit() const
{
    return isdigit(peek());
}

bool InputStream::isWhiteSpace() const
{
    return (isspace(peek()));
}

bool InputStream::isID() const
{
    char c = peek();

    return (c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

void InputStream::consume()
{
    if (isEof())
    {
        return;
    }
    else if (isNewLine())
    {
        current.line++;
        current.col = 1;
    }
    else
    {
        current.col++;
    }

    current.ptr++;
}

std::string InputStream::location() const
{
    return eucleia::stringify("[line: %4d, col: %3d]", current.line, current.col);
}
