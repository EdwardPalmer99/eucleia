/**
 * @file CharStream.hpp
 * @author Edward Palmer
 * @date 2025-01-16
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include <filesystem>
#include <string>
#include <unordered_map>

class CharStream
{
public:
    CharStream() = delete;
    CharStream(const std::string &path);

    ~CharStream();

    // Returns the current character in the stream.
    char current() const;

    // Increments the pointer and returns the new current char.
    char increment();

    // Decrements the pointer and returns the new current char.
    char decrement();

    // Returns true if there are no more characters to read.
    bool isLast() const;

    bool isNewLine() const;

    bool isQuote() const;

    bool isComment();

    bool isShebang();

    bool isPunctuation() const;

    bool isWhiteSpace() const;

    bool isDigit() const;

    bool isOperator() const;

    bool isID() const;

    inline unsigned int currentLine() const { return line; }

    inline unsigned int currentCol() const { return col; }

    // Prints stream's location.
    std::string location() const;

private:
    unsigned int endCol(unsigned int lineNum) const;

    const std::filesystem::path path;

    char *base{nullptr};
    char *ptr{nullptr};

    unsigned int line;
    unsigned int col;

    // Required to handle user repeatedly decrementing.
    std::unordered_map<unsigned int, unsigned int> lineColCount;
};
