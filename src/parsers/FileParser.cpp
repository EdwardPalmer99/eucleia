//
//  FileParser.cpp
//  Eucleia
//
//  Created by Edward on 20/01/2024.
//

#include "FileParser.hpp"
#include "Exceptions.hpp"
#include "Grammar.hpp"
#include "Logger.hpp"
#include "NodeFactory.hpp"
#include "ParserData.hpp"
#include <assert.h>
#include <cassert>
#include <cstring>
#include <memory>
#include <stdlib.h>


AnyNode::Ptr FileParser::parseMainFile(const std::string entryPointPath_)
{
    /* Clear data for all imports */
    ParserData::instance().clearImports();

    /* Create parser for entry-point file. If required, it will create additional parsers for other imported files */
    return FileParser(entryPointPath_).buildAST();
}


FileParser::FileParser(const std::string &fpath)
    : _parentDirPath(buildParentDirPath(fpath)),
      _tokens(Tokenizer::build(fpath)),
      _subParsers(*this)
{
}


AnyNode::Ptr FileParser::buildAST()
{
    BaseNodePtrVector nodes;

    while (!tokens().empty())
    {
        auto node = parseExpression();

        nodes.emplace_back(node);

        skipSemicolonLineEndingIfRequired(*node);
    }

    nodes.shrink_to_fit();

    return NodeFactory::createFileNode(std::move(nodes));
}


BaseNode::Ptr FileParser::maybeBinary(BaseNode::Ptr leftExpression, int leftPrecedence)
{
    // Special case: tokens empty. Cannot be binary expression.
    if (tokens().empty())
    {
        return leftExpression;
    }

    // Take a peek look at the next token. Is it an operator token. If it is then
    // we need to create a binary node.
    Token next = tokens().front(); /* TODO: - should wrap so alerts if no more tokens */

    if (next.type() == Token::Operator)
    {
        // Yes it is an operator. Does it have a higher precedence than the
        // left-hand expression?
        const int nextPrecedence = getPrecedence();
        if (nextPrecedence > leftPrecedence)
        {
            tokens().pop(); // Move along one.

            auto rightExpression = maybeBinary(parseAtomically(), nextPrecedence);

            // Create binary or assign node.
            BaseNode::Ptr node{nullptr};

            bool isAssignNode = (next == std::string("="));

            if (isAssignNode)
                node = NodeFactory::createAssignNode(leftExpression, rightExpression);
            else
                node = std::make_shared<BinaryNode>(leftExpression, rightExpression, next);

            // Wrap binary node by calling ourselves should the next operator
            // be of a greater precedence.
            return maybeBinary(std::move(node), leftPrecedence);
        }
    }

    // Default.
    return leftExpression;
}


BaseNode::Ptr FileParser::maybeFunctionCall(ParseMethod expression) /* TODO: - remove */
{
    auto expr = expression(); // Possible function name.

    return equals(Token::Punctuation, "(") ? _subParsers.function.parseFunctionCall(expr) : expr;
}


BaseNode::Ptr FileParser::maybeArrayAccess(ParseMethod expression)
{
    auto expr = expression(); // Possible array variable name.

    return equals(Token::Punctuation, "[") ? _subParsers.dataType.parseArrayAccessor(expr) : expr;
}


BaseNode::Ptr FileParser::maybeFunctionCallOrArrayAccess(ParseMethod expression) /* REMOVE */
{
    auto expr = expression();
    assert(expr != nullptr);

    if (!tokens().empty())
    {
        if (equals(Token::Punctuation, "("))
            return _subParsers.function.parseFunctionCall(expr); /* TODO: - remove */
        else if (equals(Token::Punctuation, "["))
            return _subParsers.dataType.parseArrayAccessor(expr);
        else if (equals(Token::Punctuation, "."))
            return _subParsers.classParser.parseStructAccessor(expr);
    }

    return expr;
}


BaseNode::Ptr FileParser::parseExpression()
{
    return maybeFunctionCallOrArrayAccess(std::bind(&FileParser::parseExpressionHelper, this));
}


BaseNode::Ptr FileParser::parseExpressionHelper()
{
    return maybeBinary(parseAtomically(), 0);
}


BaseNode::Ptr FileParser::parseAtomically()
{
    // 1. Call parseAtomicallyExpression() -> This may be the function name.
    // 2. Do we have a next token which is "("? --> YES --> FUNCTION CALL!
    return maybeFunctionCallOrArrayAccess(std::bind(&FileParser::parseAtomicallyExpression, this));
}


BaseNode::Ptr FileParser::parseAtomicallyExpression()
{
    // TODO: - breakup
    if (equals(Token::Punctuation, "("))
        return parseBrackets();
    else if (equals(Token::Punctuation, "["))
        return _subParsers.dataType.parseArray();
    else if (equals(Token::Punctuation, "{"))
        return _subParsers.block.parseBlock();
    else if (equals(Token::Keyword, "true") || equals(Token::Keyword, "false"))
        return _subParsers.dataType.parseBool();
    else if (equals(Token::Keyword, "while"))
        return _subParsers.loop.parseWhile();
    else if (equals(Token::Keyword, "do"))
        return _subParsers.loop.parseDoWhile();
    else if (equals(Token::Keyword, "for"))
        return _subParsers.loop.parseFor();
    else if (equals(Token::Keyword, "if"))
        return _subParsers.controlFlow.parseIf();
    else if (equals(Token::Keyword, "import"))
        return _subParsers.import.parseImport();
    else if (equals(Token::Keyword, "func")) // Functions should be defined as in C --> will need void type
        return _subParsers.function.parseFunctionDefinition();
    else if (equals(Token::Keyword, "class"))
        return _subParsers.classParser.parseClass();
    else if (Grammar::instance().isDataType(tokens().front()))
        return _subParsers.variable.parseVariableDefinition();
    else if (equals(Token::Keyword, "break"))
        return _subParsers.controlFlow.parseBreak();
    else if (equals(Token::Keyword, "return"))
        return _subParsers.controlFlow.parseReturn();

    // TODO: - split-up into separate method for unary operators.
    // Parse unary operators.
    else if (equals(Token::Operator, "!"))
        return _subParsers.unary.parseNot();
    // Parse prefix increment operator i.e.
    else if (equals(Token::Operator, "++"))
        return _subParsers.unary.parsePrefixIncrement();
    else if (equals(Token::Operator, "--"))
        return _subParsers.unary.parsePrefixDecrement();
    else if (equals(Token::Operator, "-"))
        return _subParsers.unary.parseNegation();

    const Token &token = tokens().front();

    switch (token.type())
    {
        case Token::Variable:
            return _subParsers.variable.parseVariable();
        case Token::String:
            return _subParsers.dataType.parseString();
        case Token::Int:
            return _subParsers.dataType.parseInt();
        case Token::Float:
            return _subParsers.dataType.parseFloat();
        default:
            ThrowException("unexpected token: " + token);
    }
}


void FileParser::skipSemicolonLineEndingIfRequired(const BaseNode &node)
{
    switch (node.type())
    {
        case NodeType::Module:
        case NodeType::File:
        case NodeType::If:
        case NodeType::ForLoop:
        case NodeType::While:
        case NodeType::DoWhile:
        case NodeType::Block:
        case NodeType::Program:
        case NodeType::Function:
            break;
        default:
            skip(";");
            break;
    };
}


int FileParser::getPrecedence()
{
    const Token &token = tokens().front();

    if (token.type() != Token::Operator)
    {
        return 0;
    }

    if (token == "=")
        return 1;
    else if (token == "||")
        return 2;
    else if (token == "&&")
        return 3;
    else if ((token == "<") ||
             (token == ">") ||
             (token == "<=") ||
             (token == ">=") ||
             (token == "==") ||
             (token == "!="))
        return 7;
    else if ((token == "+") ||
             (token == "-"))
        return 10;
    else if ((token == "*") ||
             (token == "/") ||
             (token == "%"))
        return 20;
    else
        return 0;
}


BaseNode::Ptr FileParser::parseBrackets()
{
    skip("(");

    BaseNode::Ptr expression = parseExpression();

    skip(")");

    return expression;
}


std::string FileParser::buildParentDirPath(const std::string &filePath_) const
{
    for (long i = (long)filePath_.size() - 1; i != 0; --i)
    {
        char c = filePath_.at(i);

        if (c == '/') /* Hit our first '/' (successfully stripped file name and extension) */
        {
            return filePath_.substr(0, i + 1);
        }
    }

    return "."; /* Failed */
}
