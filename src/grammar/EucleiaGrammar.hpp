//
//  EucleiaGrammar.hpp
//  Eucleia
//
//  Created by Edward on 03/02/2024.
//

#pragma once
#include <string>
#include <unordered_set>

class EucleiaGrammar
{
public:
    static EucleiaGrammar &instance();

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
    EucleiaGrammar();

private:
    const std::unordered_set<char> punctuation;
    const std::unordered_set<char> operators;

    const std::unordered_set<std::string> keywords;
    const std::unordered_set<std::string> dataTypes;
};
