//
//  EucleiaParser.cpp
//  Eucleia
//
//  Created by Edward on 20/01/2024.
//

#include "EucleiaParser.hpp"
#include "EucleiaUtility.hpp"
#include <stdlib.h>
#include <assert.h>

Parser::Parser(const std::string & fpath) : _tokenizer{fpath} {}


#pragma mark -

std::shared_ptr<FileNode> Parser::buildAbstractSymbolTree()
{
	std::vector<std::shared_ptr<BaseNode>> nodes;
		
	while (!_tokenizer.empty() && peekToken().type != Token::None)
	{
		auto node = parseExpression();
		
		auto nodeType = node ? node->type() : BaseNode::None;
			
		nodes.push_back(std::move(node));
		
		skipSemicolonLineEndingIfRequired(nodeType);
	}
	
	nodes.shrink_to_fit();
	
	return std::make_shared<FileNode>(nodes);
}


std::shared_ptr<FileNode> Parser::buildAbstractSymbolTree(const std::string & fpath)
{
	Parser parser(fpath);
	
	return parser.buildAbstractSymbolTree();
}



/// import "path_to_some_file"
///
/// Imports a file and its functions into this scope.
std::shared_ptr<FileNode> Parser::parseImport()
{
	skipKeyword("import");
	
	// Read the file path:
	auto token = nextToken();
	assert(token.type == Token::String);
	
	auto ast = Parser::buildAbstractSymbolTree(token.value);
	if (!ast)
	{
		printWarpError("Failed to import file with path '%s'.", token.value.c_str());
	}
	
	return ast;
}


#pragma mark - *** Simple Types ***

/// parseProgram
///
/// Parses a sequence of expression. If there is only one expression within the
/// brackets then we just return that.
/// TODO: - add handling for no expressions within the brackets.
std::shared_ptr<BaseNode> Parser::parseProgram()
{
	auto program = parseProgramLines();
			
	if (program->nodes.size() == 1)	// Return single node (more efficient).
	{
		return std::move(program->nodes.front());
	}
	
	return program;
}


std::shared_ptr<IntNode> Parser::parseInt()
{
	Token token = nextToken();
	
	long intValue = strtold(token.value.c_str(), NULL);
	
	return std::make_shared<IntNode>(intValue);
}


std::shared_ptr<FloatNode> Parser::parseFloat()
{
	Token token = nextToken();
	
	double floatValue = strtof(token.value.c_str(), NULL);
	
	return std::make_shared<FloatNode>(floatValue);
}


std::shared_ptr<BoolNode> Parser::parseBool()
{
	Token token = nextToken();
	
	bool state = (token.value == "true");
	
	return std::make_shared<BoolNode>(state);
}


std::shared_ptr<StringNode> Parser::parseString()
{
	Token token = nextToken();
	
	return std::make_shared<StringNode>(token.value);
}


std::shared_ptr<BaseNode> Parser::parseBrackets()
{
	skipPunctuation("(");
	
	auto expression = parseExpression();
	
	skipPunctuation(")");
	
	return expression;
}


/// Variable definition:
/// int/float/string/bool/array [VARIABLE_NAME]
std::shared_ptr<BaseNode> Parser::parseVariableDefinition()
{
	Token typeToken = nextToken();
	assert(typeToken.type == Token::Keyword);
		
	Token nameToken = nextToken();
	assert(nameToken.type == Token::Variable);
	
	std::string & typeName = typeToken.value;
	std::string & variableName = nameToken.value;
	
	// TODO: - add void typename for functions eventually.
	if (typeName == "int")
		return std::make_shared<VariableNode>(variableName, VariableNode::Type::Int);
	else if (typeName == "float")
		return std::make_shared<VariableNode>(variableName, VariableNode::Type::Float);
	else if (typeName == "bool")
		return std::make_shared<VariableNode>(variableName, VariableNode::Type::Bool);
	else if (typeName == "string")
		return std::make_shared<VariableNode>(variableName, VariableNode::Type::String);
	else if (typeName == "array")
		return std::make_shared<VariableNode>(variableName, VariableNode::Type::Array);
	else
		printWarpError("expected variable type for variable %s.\n", typeName.c_str());
}


/// [VARIABLE_NAME]
std::shared_ptr<BaseNode> Parser::parseVariableName()
{
	Token token = nextToken();
	assert(token.type == Token::Variable);

	return std::make_shared<VariableNameNode>(token.value);
}


#pragma mark - *** Loops ***

/// do
/// {
/// 	[code]
/// }
/// while ([condition is true]);
std::shared_ptr<DoWhileNode> Parser::parseDoWhile()
{
	skipKeyword("do");
	auto body = parseProgram();
	skipKeyword("while");
	auto condition = parseBrackets();
	
	return std::make_shared<DoWhileNode>(std::move(condition), std::move(body));
}


/// while ([condition is true])
/// {
/// 	[code]
/// };
std::shared_ptr<WhileNode> Parser::parseWhile()
{
	skipKeyword("while");
	
	auto condition = parseBrackets();
	auto body = parseProgram();
	
	return std::make_shared<WhileNode>(std::move(condition), std::move(body));
}


/// for ([start]; [condition]; [update])
/// {
/// 	[code]
/// };
std::shared_ptr<ForLoopNode> Parser::parseFor()
{
	skipKeyword("for");
		
	auto brackets = parseDelimited("(", ")", ";", std::bind(&Parser::parseExpression, this));
	
	if (brackets->nodes.size() != 3)
	{
		printWarpError("Expected 3 arguments for for-loop but got %ld\n", brackets->nodes.size());
	}
	
	auto start 		= brackets->nodes[0];
	auto condition	= brackets->nodes[1];
	auto update 	= brackets->nodes[2];
	auto body 		= parseProgram();
	
	return std::make_shared<ForLoopNode>(start, condition, update, body);
}


#pragma mark - *** Control Flow ***

std::shared_ptr<IfNode> Parser::parseIf()
{
	// For now, only permit a single if statement.
	skipKeyword("if");
	
	auto condition = parseBrackets();
	auto thenDo = parseProgram();
	
	
		
	std::shared_ptr<BaseNode>elseDo{nullptr};	// Optional.
	if (peekToken().value == "else")
	{
		skipKeyword("else");
		
		// Option 1: else if (condition) { [statement]; }
		if (peekToken().value == "if")
			elseDo = parseIf();
		// Option 2: else { [statement]; }
		else
			elseDo = parseProgram();
	}
	
	return std::make_shared<IfNode>(condition, thenDo, elseDo);
}


std::shared_ptr<BreakNode> Parser::parseBreak()
{
	skipKeyword("break");
		
	return std::make_shared<BreakNode>();
}


std::shared_ptr<ReturnNode> Parser::parseReturn()
{
	skipKeyword("return");
	
	std::shared_ptr<BaseNode> returnedExpression{nullptr};
	
	if (!isPunctuation(";"))
	{
		returnedExpression = parseExpression();
	}
	
	return std::make_shared<ReturnNode>(returnedExpression);
}


#pragma mark - *** Functions ***

/// func test(a, b, c)
/// {
///		[BODY]
/// };
std::shared_ptr<FunctionNode> Parser::parseFunctionDefinition()
{
	skipKeyword("func");
	
	auto funcName = parseVariableName();
	auto funcArgs = parseDelimited("(", ")", ",", std::bind(&Parser::parseVariableDefinition, this));	// Func variables.
	auto funcBody = parseProgram();
	
	return std::make_shared<FunctionNode>(funcName, funcArgs, funcBody);
}


///
/// Example: test(a, b, c);
///
std::shared_ptr<FunctionCallNode> Parser::parseFunctionCall(std::shared_ptr<BaseNode> lastExpression)
{
	auto functionName = std::move(lastExpression);
	auto functionArgs = parseDelimited("(", ")", ",", std::bind(&Parser::parseExpression, this));
	
	return std::make_shared<FunctionCallNode>(functionName, functionArgs);
}


#pragma mark - *** Arrays ***

///
/// Expression: array_variable[0].
///
std::shared_ptr<ArrayAccessNode> Parser::parseArrayAccessor(std::shared_ptr<BaseNode> lastExpression)
{
	auto arrayName = std::move(lastExpression);

	skipPunctuation("[");
	
	auto arrayIndex = parseExpression();
	
	skipPunctuation("]");
		
	return std::make_shared<ArrayAccessNode>(arrayName, arrayIndex);
}


/// [1, 2, 3, 4] OR [true, false, true] OR [1.2, 2.4] OR ["hello, ", "world!"].
std::shared_ptr<ArrayNode> Parser::parseArray()
{
	auto nodes = parseDelimited("[", "]", ",", std::bind(&Parser::parseExpression, this));
	
	return std::make_shared<ArrayNode>(*nodes);
}


#pragma mark - *** Assignment/Binary ***

std::shared_ptr<BaseNode> Parser::maybeBinary(std::shared_ptr<BaseNode> leftExpression, int leftPrecedence)
{
	// Take a peek look at the next token. Is it an operator token. If it is then
	// we need to create a binary node.
	Token & next = peekToken();

	if (next.type == Token::Operator)
	{
		// Yes it is an operator. Does it have a higher precedence than the
		// left-hand expression?
		const int nextPrecedence = getPrecedence();
		if (nextPrecedence > leftPrecedence)
		{
			skipToken();	// Move along one.

			auto rightExpression = maybeBinary(parseAtomically(), nextPrecedence);
			
			// Create binary or assign node.
			std::shared_ptr<BaseNode> node{nullptr};
			
			bool isAssignNode = (next.value == "=");

			if (isAssignNode)
				node = std::make_shared<AssignNode>(std::move(leftExpression), std::move(rightExpression));
			else
				node = std::make_shared<BinaryNode>(leftExpression, rightExpression, next.value);

			// Wrap binary node by calling ourselves should the next operator
			// be of a greater precedence.
			return maybeBinary(std::move(node), leftPrecedence);
		}
	}
	
	// Default.
	return leftExpression;
}


#pragma mark - *** Maybe ***

std::shared_ptr<BaseNode> Parser::maybeFunctionCall(ParseMethod expression)
{
	auto expr = expression();	// Possible function name.
	
	return isPunctuation("(") ? parseFunctionCall(std::move(expr)) : expr;
}


std::shared_ptr<BaseNode> Parser::maybeArrayAccess(ParseMethod expression)
{
	auto expr = expression();	// Possible array variable name.
		
	return isPunctuation("[") ? parseArrayAccessor(std::move(expr)) : expr;
}


std::shared_ptr<BaseNode> Parser::maybeFunctionCallOrArrayAccess(ParseMethod expression)
{
	auto expr = expression();
	
	if (isPunctuation("("))
		return parseFunctionCall(std::move(expr));
	else if (isPunctuation("["))
		return parseArrayAccessor(std::move(expr));
	else
		return expr;
}


#pragma mark - *** Utility ***

std::shared_ptr<BaseNode> Parser::parseExpression()
{
	return maybeFunctionCallOrArrayAccess(std::bind(&Parser::parseExpressionHelper, this));
}


std::shared_ptr<BaseNode> Parser::parseExpressionHelper()
{
	return maybeBinary(parseAtomically(), 0);
}


std::shared_ptr<BaseNode> Parser::parseAtomically()
{
	// 1. Call parseAtomicallyExpression() -> This may be the function name.
	// 2. Do we have a next token which is "("? --> YES --> FUNCTION CALL!
	return maybeFunctionCallOrArrayAccess(std::bind(&Parser::parseAtomicallyExpression, this));
}


std::shared_ptr<BaseNode> Parser::parseAtomicallyExpression()
{
	// TODO: - breakup 
	if (isPunctuation("("))
		return parseBrackets();
	else if (isPunctuation("["))
		return parseArray();
	else if (isPunctuation("{"))
		return parseProgram();
	else if (isKeyword("true") || isKeyword("false"))
		return parseBool();
	else if (isKeyword("while"))
		return parseWhile();
	else if (isKeyword("do"))
		return parseDoWhile();
	else if (isKeyword("for"))
		return parseFor();
	else if (isKeyword("if"))
		return parseIf();
	else if (isKeyword("import"))
		return parseImport();
	else if (isKeyword("func"))	// Functions should be defined as in C --> will need void type
		return parseFunctionDefinition();
	else if (isDataTypeKeyword())
		return parseVariableDefinition();
	else if (isKeyword("break"))
		return parseBreak();
	else if (isKeyword("return"))
		return parseReturn();
	
	// TODO: - split-up into separate method for unary operators.
	// Parse unary operators.
	else if (isOperator("!"))
		return parseNot();
	// Parse prefix increment operator i.e.
	else if (isOperator("++"))
		return parsePrefixIncrement();
	else if (isOperator("--"))
		return parsePrefixDecrement();
	else if (isOperator("-"))
		return parseNegation();
	
	Token & token = peekToken();
	
	switch (token.type)
	{
		case Token::Variable:
			return parseVariableName();
		case Token::String:
			return parseString();
		case Token::Int:
			return parseInt();
		case Token::Float:
			return parseFloat();
		default:
			unexpectedToken();
			exit(EXIT_FAILURE);
	}
}
	

std::shared_ptr<NotNode> Parser::parseNot()
{
	skipOperator("!");
		
	return std::make_shared<NotNode>(parseAtomically());
}


std::shared_ptr<PrefixIncrementNode> Parser::parsePrefixIncrement()
{
	skipOperator("++");
	
	return std::make_shared<PrefixIncrementNode>(parseAtomically());
}


std::shared_ptr<PrefixDecrementNode> Parser::parsePrefixDecrement()
{
	skipOperator("--");
	
	return std::make_shared<PrefixDecrementNode>(parseAtomically());
}


std::shared_ptr<NegationNode> Parser::parseNegation()
{
	skipOperator("-");
	
	return std::make_shared<NegationNode>(parseAtomically());
}


/// parseDelimited
///
/// Parse everything between start using the provided parser and ignoring the separators.
///
/// **Example:**
///
/// function aFunction(a, b, c) --> start = (, stop = ), separator = ,
///
std::shared_ptr<ProgramNode> Parser::parseDelimited(std::string start,
												 std::string stop,
												 std::string separator,
												 std::function<std::shared_ptr<BaseNode>(void)> parseMethod)
{
	skipPunctuation(start);	// Skip the punctuation at the start.
	
	std::vector<std::shared_ptr<BaseNode>> parsedNodes;
	
	// Iterate while we still have tokens and haven't reached stop token.
	bool firstCall = true;
		
	while (!_tokenizer.empty() && !isPunctuation(stop))
	{
		// Skip separator on each subsequent call (i.e. a, b)
		if (firstCall)
		{
			firstCall = false;
		}
		else
		{
			skipPunctuation(separator);
		}
		
		if (isPunctuation(stop))
		{
			break;
		}
				
		// Parse the token.
		parsedNodes.push_back(parseMethod());
	}

	skipPunctuation(stop);	// Skip the punctuation at the end.
	
	// Resize to fit.
	parsedNodes.shrink_to_fit();
	
	return std::make_shared<ProgramNode>(parsedNodes);
}


void Parser::skipSemicolonLineEndingIfRequired(BaseNode::NodeType expressionType)
{
	switch (expressionType)
	{
		case BaseNode::File:
		case BaseNode::Program:
		case BaseNode::If:
		case BaseNode::While:
		case BaseNode::DoWhile:
		case BaseNode::ForLoop:
		case BaseNode::Function:
			break;
		default:
			skipPunctuation(";");
			break;
	}
}


std::shared_ptr<ProgramNode> Parser::parseProgramLines()
{
	skipPunctuation("{");
	
	std::vector<std::shared_ptr<BaseNode>> parsedNodes;
		
	while (!_tokenizer.empty() && !isPunctuation("}"))
	{
		auto expression = parseExpression();
		
		auto expressionType = expression ? expression->type() : BaseNode::None;
		
		parsedNodes.push_back(std::move(expression));
		
		skipSemicolonLineEndingIfRequired(expressionType);
	}
	
	skipPunctuation("}");
	
	return std::make_shared<ProgramNode>(std::move(parsedNodes));
}


int Parser::getPrecedence()
{
	Token & token = peekToken();
	
	if (token.type != Token::Operator)
	{
		return 0;
	}
	
	if (token.value == "=")
		return 1;
	else if (token.value == "||")
		return 2;
	else if (token.value == "&&")
		return 3;
	else if ((token.value == "<") ||
			 (token.value == ">") ||
			 (token.value == "<=") ||
			 (token.value == ">=") ||
			 (token.value == "==") ||
			 (token.value == "!="))
		return 7;
	else if ((token.value == "+") || 
			 (token.value == "-"))
		return 10;
	else if ((token.value == "*") ||
			 (token.value == "/") ||
			 (token.value == "%"))
		return 20;
	else
		return 0;
}


#pragma mark - *** is... ***

bool Parser::isKeyword(const std::string & keyword)
{
	Token & token = peekToken();
	
	return (token.type == Token::Keyword && token.value == keyword);
}


bool Parser::isDataTypeKeyword()
{
	return (_tokenizer.isDataTypeToken());
}


bool Parser::isPunctuation(const std::string & punctuation)
{
	Token & token = peekToken();
	
	return (token.type == Token::Punctuation && token.value == punctuation);
}


bool Parser::isOperator(const std::string & operatorName)
{
	Token & token = peekToken();
	
	return (token.type == Token::Operator && token.value == operatorName);
}


#pragma mark - *** skip... ***

void Parser::skipKeyword(const std::string & name)
{
	if (!isKeyword(name))
	{
		printWarpError("expected keyword '%s'.\n", name.c_str());
	}

	skipToken();
}


void Parser::skipPunctuation(const std::string & name)
{
	if (!isPunctuation(name))
	{
		printWarpError("expected punctuation '%s'.\n", name.c_str());
	}

	skipToken();
}


void Parser::skipOperator(const std::string & name)
{
	if (!isOperator(name))
	{
		printWarpError("expected operator '%s'.\n", name.c_str());
	}

	skipToken();
}


#pragma mark -

void Parser::unexpectedToken()
{
	Token & token = peekToken();
	
	printWarpError("Unexpected token of type '%s' and value '%s'.\n", token.description().c_str(), token.value.c_str());
}


void Parser::assertNodeType(const std::shared_ptr<BaseNode> & node, BaseNode::NodeType expectedType)
{
	assert(node->type() == expectedType);
}
