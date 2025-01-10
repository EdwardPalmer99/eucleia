//
//  EucleiaGrammar.hpp
//  Eucleia
//
//  Created by Edward on 03/02/2024.
//

#ifndef EucleiaGrammar_hpp
#define EucleiaGrammar_hpp

#include <memory>
#include <string>
#include <unordered_set>

class EucleiaGrammar
{
public:
    EucleiaGrammar();
    ~EucleiaGrammar() = default;

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

private:
    std::unordered_set<char> punctuation;
    std::unordered_set<char> operators;

    std::unordered_set<std::string> keywords;
    std::unordered_set<std::string> dataTypes;
};

#endif /* EucleiaGrammar_hpp */
