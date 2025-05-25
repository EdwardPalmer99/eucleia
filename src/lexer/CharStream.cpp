/**
 * @file CharStream.cpp
 * @author Edward Palmer
 * @date 2025-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "CharStream.hpp"
#include "Exceptions.hpp"
#include "FileUtils.hpp"
#include "Grammar.hpp"
#include "Stringify.hpp"
#include <algorithm>
#include <cstring>
#include <filesystem>
#include <stdio.h>
#include <stdlib.h>

CharStream::CharStream(const std::string &path_) : path(path_)
{
    base = ptr = eucleia::loadFileContents(path.c_str());
    line = col = 1;
}


CharStream::~CharStream()
{
    if (base != nullptr)
    {
        free(base);
    }
}


char CharStream::current() const
{
    if (!ptr)
        ThrowException(location() + ": char pointer is NULL");

    return (*ptr);
}


char CharStream::increment()
{
    if (isLast())
        ThrowException(location() + ": cannot increment char pointer");

    if (isNewLine()) // Before increment, current == '\n'
    {
        lineColCount[line++] = col; // NB: postfix not prefix operator!
        col = 1;
    }
    else
    {
        ++col;
    }

    return *(++ptr);
}


char CharStream::decrement()
{
    if (!ptr || ptr == base)
        ThrowException(location() + ": cannot decrement char pointer");

    (void)(--ptr);

    if (isNewLine())
    {
        --line;
        col = endCol(line); // Get end column number.
    }
    else
    {
        --col;
    }

    return current();
}


bool CharStream::isLast() const
{
    return (current() == '\0');
}


bool CharStream::isNewLine() const
{
    return (current() == '\n');
}


bool CharStream::isQuote() const
{
    return (current() == '"');
}


bool CharStream::isComment()
{
    if (isLast() || current() != '/')
        return false;

    /* Find the next '/' for comment begin */
    char next = increment();
    decrement(); // Reset the pointer.

    return (next == '/');
}


bool CharStream::isShebang()
{
    /* Special UNIX char sequence '#!' */

    if (isLast() || current() != '#')
        return false;

    /* Find the next '!' for shebang */
    char next = increment();
    decrement(); // Reset the pointer.

    return (next == '!');
}


bool CharStream::isOperator() const
{
    return Grammar::instance().isOperator(current());
}


bool CharStream::isPunctuation() const
{
    return Grammar::instance().isPunctuation(current());
}


bool CharStream::isDigit() const
{
    return isdigit(current());
}


bool CharStream::isWhiteSpace() const
{
    return (isspace(current()));
}


bool CharStream::isID() const
{
    char c = current();

    return (c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}


unsigned int CharStream::endCol(unsigned int lineNum) const
{
    auto iter = lineColCount.find(lineNum);
    if (iter == lineColCount.end())
        ThrowException(location() + eucleia::stringify(": no column count for line %u", lineNum));

    return iter->second;
}


std::string CharStream::location() const
{
    return eucleia::stringify("(%s:%d:%d)", path.filename().c_str(), line, col);
}