//
//  RecognizedInputs.hpp
//  Eucleia
//
//  Created by Edward on 03/02/2024.
//

#ifndef RecognizedInputs_hpp
#define RecognizedInputs_hpp

#include <memory>
#include <set>
#include <string>

class RecognizedInputs
{
public:
	RecognizedInputs();
	~RecognizedInputs() = default;
	
	inline bool isDataType(const std::string & value) const { return _dataTypes.contains(value); }
	inline bool isKeyword(const std::string & value) const { return _keywords.contains(value); }
	inline bool isOperator(char c) const { return _operators.contains(c); }
	inline bool isPunctuation(char c) const { return _punctuation.contains(c); }

protected:
	inline const std::set<char> & punctuation() const { return _punctuation; }
	inline const std::set<char> & operators() const { return _operators; }
	inline const std::set<std::string> & keywords() const { return _keywords; }
	inline const std::set<std::string> & dataTypes() const { return _dataTypes; }

private:
	std::set<char> _punctuation;
	std::set<char> _operators;
	
	std::set<std::string> _extraOperators;
	
	std::set<std::string> _keywords;
	std::set<std::string> _dataTypes;
};

#endif /* RecognizedInputs_hpp */


