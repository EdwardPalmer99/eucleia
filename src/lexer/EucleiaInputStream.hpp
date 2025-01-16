/**
 * @file EucleiaInputStream.hpp
 * @author Edward Palmer
 * @date 2025-01-16
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include <string>

class InputStream
{
public:
    InputStream() = delete;
    InputStream(const std::string &path);

    ~InputStream();

    // Consumes token and reads the next character.
    char next();

    // Peeks the next character.
    char peek() const;

    // Peeks the next two characters.
    char *peek2(); // TODO: - bad to use static variable if multithreading.

    bool isEof() const;
    bool isNewLine() const;
    bool isStringStart() const;

    // Prints stream's location.
    std::string location() const;

protected:
    bool isComment();
    bool isPunctuation() const;
    bool isWhiteSpace() const;
    bool isDigit() const;
    bool isOperator() const;
    bool isID() const;

private:
    // Consumes token and updates stream info.
    void consume();

    struct FileContents
    {
        // Pointer to start of file.
        char *base{nullptr};

        // Pointer to stream position.
        char *ptr{nullptr};

        // Track line and column.
        int line{1};
        int col{1};
    } file;

    // Path to the file.
    const std::string path;
};
