//
//  EucleiaGrammar.hpp
//  Eucleia
//
//  Created by Edward on 03/02/2024.
//

#ifndef EucleiaGrammar_hpp
#define EucleiaGrammar_hpp

#include <memory>
#include <set>
#include <string>

class EucleiaGrammar
{
  public:
    EucleiaGrammar();
    ~EucleiaGrammar() = default;

    inline bool isDataType(const std::string &value) const
    {
        return _dataTypes.count(value);
    }
    inline bool isKeyword(const std::string &value) const
    {
        return _keywords.count(value);
    }
    inline bool isOperator(char c) const
    {
        return _operators.count(c);
    }
    inline bool isPunctuation(char c) const
    {
        return _punctuation.count(c);
    }

  protected:
    inline const std::set<char> &punctuation() const
    {
        return _punctuation;
    }
    inline const std::set<char> &operators() const
    {
        return _operators;
    }
    inline const std::set<std::string> &keywords() const
    {
        return _keywords;
    }
    inline const std::set<std::string> &dataTypes() const
    {
        return _dataTypes;
    }

  private:
    std::set<char> _punctuation;
    std::set<char> _operators;

    std::set<std::string> _keywords;
    std::set<std::string> _dataTypes;
};

#endif /* EucleiaGrammar_hpp */
