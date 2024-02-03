//
//  EucleiaTokenizer.hpp
//  Eucleia
//
//  Created by Edward on 18/01/2024.
//

#ifndef EucleiaTokenizer_hpp
#define EucleiaTokenizer_hpp

#include <string>
#include <set>
#include <queue>
#include "EucleiaInputStream.hpp"

// TODO: - bang in a namespace.

struct Token
{
	enum TokenType
	{
		None,
		Punctuation,
		Keyword,
		Variable,
		String,
		Operator,
		Int,
		Float,
		Bool
	};
		
	Token(TokenType _type, std::string && _value) : type{_type}, value{_value} {}
	Token(TokenType _type, std::string & _value) : type{_type}, value{_value} {}
	
	static Token blank() { return Token(None, ""); }
		
	std::string description() const;
	
	TokenType type;
	std::string value;
};


class Tokenizer : public InputStream
{
public:
	Tokenizer() = delete;
	Tokenizer(const std::string & fpath);
	~Tokenizer() = default;
	
	Token & peek();
	Token next();
	
	inline bool empty() const { return _tokens.empty(); }
	
	bool isDataTypeToken();
	
protected:
	void skipComment();
	void skipWhitespace();
			
	// TODO: - should be const methods.
	Token readString();
	Token readNumber();
	Token readID();
	Token readOperator();
	Token readPunctuation();
		
	bool isKeyword(const std::string & possibleKeyword) const;
	
	void generateTokens();
	Token buildNextToken();

private:
	std::set<std::string> _allowedKeywords;
	std::queue<Token> _tokens;
};

#endif /* EucleiaTokenzier_hpp */
