/**
 * @file Grammar.hpp
 * @author Edward Palmer
 * @date 2025-01-10
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "SingletonT.hpp"
#include <string>
#include <unordered_set>

class GrammarImpl
{
public:
    bool isDataType(const std::string &value) { return dataTypes.count(value); }
    bool isKeyword(const std::string &value) { return keywords.count(value); }
    bool isOperator(char c) { return operators.count(c); }
    bool isPunctuation(char c) { return punctuation.count(c); }

protected:
    friend class SingletonT<GrammarImpl>; /* Can use constructor */
    GrammarImpl();                        /* Prevent initialization except in Singleton */

private:
    using StringSet = std::unordered_set<std::string>;
    using CharSet = std::unordered_set<char>;

    const CharSet punctuation;
    const CharSet operators;

    const StringSet keywords;
    const StringSet dataTypes;
};

using Grammar = SingletonT<GrammarImpl>;
