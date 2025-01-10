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

    inline bool isDataType(const std::string &value) const
    {
        return dataTypes.count(value);
    }
    inline bool isKeyword(const std::string &value) const
    {
        return keywords.count(value);
    }
    inline bool isOperator(char c) const
    {
        return operators.count(c);
    }
    inline bool isPunctuation(char c) const
    {
        return punctuation.count(c);
    }

protected:
    Grammar();

private:
    const std::unordered_set<char> punctuation;
    const std::unordered_set<char> operators;

    const std::unordered_set<std::string> keywords;
    const std::unordered_set<std::string> dataTypes;
};
