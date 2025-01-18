/**
 * @file EucleiaInputStream.cpp
 * @author Edward Palmer
 * @date 2025-01-16
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "InputStream.hpp"
#include "Exceptions.hpp"
#include "FileUtils.hpp"
#include "Grammar.hpp"
#include "Stringify.hpp"
#include <algorithm>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

InputStream::InputStream(const std::string &path_) : path(path_)
{
    // Load file from path and set pointer.
    file.base = file.ptr = eucleia::loadFileContents(path.c_str());
}


InputStream::~InputStream()
{
    if (file.base != nullptr)
        free(file.base);
}

char InputStream::peek() const
{
    return (file.ptr ? *file.ptr : '\0');
}

char *InputStream::peek2()
{
    static char buffer[3];
    memset(buffer, '\0', 3);

    buffer[0] = peek();

    if (buffer[0] != '\0')
    {
        file.ptr++;
        buffer[1] = peek();
        file.ptr--;
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
        if (file.base) // Free memory now file read.
        {
            free(file.base);
            file.base = file.ptr = nullptr;
        }

        return;
    }
    else if (isNewLine())
    {
        file.line++;
        file.col = 1;
    }
    else
    {
        file.col++;
    }

    file.ptr++;
}

std::string InputStream::location() const
{
    return eucleia::stringify("File \"%s\", Ln %d, Col %d", path.c_str(), file.line, file.col);
}