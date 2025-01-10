/**
 * @file Grammar.cpp
 * @author Edward Palmer
 * @date 2025-01-10
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Grammar.hpp"

Grammar::Grammar()
    : punctuation{',', ';', '(', ')', '{', '}', '[', ']', '.'},
      operators{'+', '-', '*', '/', '%', '&', '|', '<', '>', '=', '!'},
      keywords{"if", "else", "true", "false", "func", "while", "do", "for", "int",
               "float", "bool", "array", "string", "break", "return", "import",
               "struct", "class", "extends"},
      dataTypes{"int", "float", "bool", "array", "string"}
{
}

Grammar &Grammar::instance()
{
    static Grammar instance;

    return instance;
}

bool Grammar::isDataType(const std::string &value)
{
    return instance().dataTypes.count(value);
}

bool Grammar::isKeyword(const std::string &value)
{
    return instance().keywords.count(value);
}

bool Grammar::isOperator(char c)
{
    return instance().operators.count(c);
}

bool Grammar::isPunctuation(char c)
{
    return instance().punctuation.count(c);
}
