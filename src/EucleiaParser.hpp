//
//  EucleiaParser.hpp
//  Eucleia
//
//  Created by Edward on 20/01/2024.
//

#ifndef EucleiaParser_hpp
#define EucleiaParser_hpp

#include "EucleiaTokenizer.hpp"
#include "EucleiaNode.hpp"
#include "EucleiaUnaryOperatorNodes.hpp"

class Parser
{
public:
	Parser() = delete;
	Parser(const std::string & fpath);
	~Parser() = default;
	
	std::shared_ptr<FileNode> buildAbstractSymbolTree();
	
	static std::shared_ptr<FileNode> buildAbstractSymbolTree(const std::string & fpath);
	
protected:
	std::shared_ptr<BoolNode> parseBool();
	std::shared_ptr<StringNode> parseString();
	std::shared_ptr<IntNode> parseInt();
	std::shared_ptr<FloatNode> parseFloat();
	
	std::shared_ptr<BaseNode> parseBrackets();
	
	std::shared_ptr<BaseNode> parseProgram();
	std::shared_ptr<ArrayNode> parseArray();

	std::shared_ptr<BaseNode> parseVariableDefinition();
	std::shared_ptr<BaseNode> parseVariableName();
	
	std::shared_ptr<DoWhileNode> parseDoWhile();
	std::shared_ptr<WhileNode> parseWhile();
	std::shared_ptr<ForLoopNode> parseFor();
	
	std::shared_ptr<IfNode> parseIf();
	std::shared_ptr<FunctionNode> parseFunctionDefinition();

	std::shared_ptr<BreakNode> parseBreak();
	std::shared_ptr<ReturnNode> parseReturn();
	
	std::shared_ptr<NotNode> parseNot();
	std::shared_ptr<PrefixIncrementNode> parsePrefixIncrement();
	std::shared_ptr<PrefixDecrementNode> parsePrefixDecrement();
	std::shared_ptr<NegationNode> parseNegation();
	
	std::shared_ptr<FileNode> parseImport();

	std::shared_ptr<ArrayAccessNode> parseArrayAccessor(std::shared_ptr<BaseNode> lastExpression);

	bool isKeyword(const std::string & keyword);
	bool isPunctuation(const std::string & punctuation);
	bool isOperator(const std::string & operatorName);
	
	bool isDataTypeKeyword();

	void skipKeyword(const std::string & keyword);
	void skipPunctuation(const std::string & punctuation);
	void skipOperator(const std::string & operatorName);


	inline Token & peekToken() { return _tokenizer.peek(); }
	inline Token nextToken() { return _tokenizer.next(); }
	inline void skipToken() { _tokenizer.next(); }
	
	void unexpectedToken();
	
	std::shared_ptr<FunctionCallNode> parseFunctionCall(std::shared_ptr<BaseNode> lastExpression);

	void skipSemicolonLineEndingIfRequired(BaseNode::NodeType expressionType);
	
	std::string parentDirectory(const std::string & fpath);


private:
	typedef std::function<std::shared_ptr<BaseNode>(void)> ParseMethod;
	
	std::shared_ptr<BaseNode> parseExpression();
	std::shared_ptr<BaseNode> parseExpressionHelper();
	std::shared_ptr<BaseNode> parseAtomically();
	std::shared_ptr<BaseNode> parseAtomicallyExpression();

	int getPrecedence(void);
	
	void assertNodeType(const std::shared_ptr<BaseNode> & node, BaseNode::NodeType expectedType);

	std::shared_ptr<BaseNode> maybeArrayAccess(ParseMethod expression);

	std::shared_ptr<BaseNode> maybeFunctionCall(ParseMethod expression);

	std::shared_ptr<BaseNode> maybeFunctionCallOrArrayAccess(ParseMethod expression);

	std::shared_ptr<ProgramNode> parseDelimited(std::string start,
												std::string stop,
												std::string separator,
												ParseMethod expression);
	
	std::shared_ptr<ProgramNode> parseProgramLines();
	
	std::shared_ptr<BaseNode> maybeBinary(std::shared_ptr<BaseNode> leftExpression, 
										  int leftPrecedence);

		
	Tokenizer _tokenizer;
	
	const std::string _parentDirectory;
};

#endif /* EucleiaParser_hpp */
