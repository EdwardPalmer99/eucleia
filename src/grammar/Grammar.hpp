/**
 * @file Grammar.hpp
 * @author Edward Palmer
 * @date 2025-01-10
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include <string>
#include <unordered_set>

class Grammar
{
public:
    static Grammar &instance();

    static bool isDataType(const std::string &value);

    static bool isKeyword(const std::string &value);

    static bool isOperator(char c);

    static bool isPunctuation(char c);

protected:
    Grammar();

private:
    const std::unordered_set<char> punctuation;
    const std::unordered_set<char> operators;

    const std::unordered_set<std::string> keywords;
    const std::unordered_set<std::string> dataTypes;
};
