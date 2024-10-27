//
//  EucleiaInputStream.hpp
//  Eucleia
//
//  Created by Edward on 18/01/2024.
//

#ifndef EucleiaInputStream_hpp
#define EucleiaInputStream_hpp

#include "EucleiaGrammar.hpp"
#include <set>
#include <string>

struct StreamPoint
{
    char *ptr{nullptr};
    int line{1};
    int col{1};
};


class InputStream
{
  public:
    InputStream() = delete;
    InputStream(const std::string fileString);

    char next();
    char peek() const;

    inline bool isEof() const
    {
        return (peek() == '\0');
    }
    inline bool isNewLine() const
    {
        return (peek() == '\n');
    }
    inline bool isStringStart() const
    {
        return (peek() == '"');
    }

  protected:
    bool isComment() const;
    bool isPunctuation() const;
    bool isWhiteSpace() const;
    bool isDigit() const;
    bool isOperator() const;
    bool isID() const;

    void reportError(const char *format, ...);

    inline const EucleiaGrammar &grammar() const
    {
        return _grammar;
    }

  private:
    void consume();

    char *_basePtr{nullptr};
    StreamPoint _current;

    EucleiaGrammar _grammar;

    const std::string _fileContents;
};


#endif /* EucleiaInputStream_hpp */
