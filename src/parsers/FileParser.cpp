//
//  FileParser.cpp
//  Eucleia
//
//  Created by Edward on 20/01/2024.
//

#include "FileParser.hpp"
#include "EucleiaModules.hpp"
#include "Exceptions.hpp"
#include "Grammar.hpp"
#include "Logger.hpp"
#include "NodeFactory.hpp"
#include "ObjectTypes.hpp"
#include "ParserData.hpp"
#include "TestModule.hpp"
#include <assert.h>
#include <cassert>
#include <cstring>
#include <memory>
#include <stdlib.h>


AnyNode FileParser::parseMainFile(const std::string entryPointPath_)
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


AnyNode FileParser::buildAST()
{
    std::vector<BaseNode *> nodes;

    while (!tokens().empty())
    {
        auto node = parseExpression();

        nodes.push_back(node);

        skipSemicolonLineEndingIfRequired(*node);
    }

    nodes.shrink_to_fit();

    return new FileNode(nodes);
}


AnyNode FileParser::maybeBinary(BaseNode *leftExpression, int leftPrecedence)
{
    // Special case: tokens empty. Cannot be binary expression.
    if (tokens().empty())
    {
        return leftExpression;
    }

    // Take a peek look at the next token. Is it an operator token. If it is then
    // we need to create a binary node.
    const Token &next = tokens().front();

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
            BaseNode *node{nullptr}; // TODO: - remove

            bool isAssignNode = (next == "=");

            if (isAssignNode)
                node = new AssignNode(leftExpression, rightExpression);
            else
                node = new BinaryNode(leftExpression, rightExpression, next);

            // Wrap binary node by calling ourselves should the next operator
            // be of a greater precedence.
            return maybeBinary(std::move(node), leftPrecedence);
        }
    }

    // Default.
    return leftExpression;
}


AnyNode FileParser::maybeFunctionCall(ParseMethod expression)
{
    auto expr = expression(); // Possible function name.

    return equals(Token::Punctuation, "(") ? _subParsers.function.parseFunctionCall(expr) : expr;
}


AnyNode FileParser::maybeArrayAccess(ParseMethod expression)
{
    auto expr = expression(); // Possible array variable name.

    return equals(Token::Punctuation, "[") ? _subParsers.dataType.parseArrayAccessor(expr) : expr;
}


AnyNode FileParser::maybeFunctionCallOrArrayAccess(ParseMethod expression)
{
    auto expr = expression();

    if (!tokens().empty())
    {
        if (equals(Token::Punctuation, "("))
            return _subParsers.function.parse(FunctionSubParser::FunctionCall, expr);
        else if (equals(Token::Punctuation, "["))
            return _subParsers.dataType.parseArrayAccessor(expr);
        else if (equals(Token::Punctuation, "."))
            return _subParsers.classParser.parseStructAccessor(expr);
    }

    return expr;
}


AnyNode FileParser::parseExpression()
{
    return maybeFunctionCallOrArrayAccess(std::bind(&FileParser::parseExpressionHelper, this));
}


AnyNode FileParser::parseExpressionHelper()
{
    return maybeBinary(parseAtomically(), 0);
}


AnyNode FileParser::parseAtomically()
{
    // 1. Call parseAtomicallyExpression() -> This may be the function name.
    // 2. Do we have a next token which is "("? --> YES --> FUNCTION CALL!
    return maybeFunctionCallOrArrayAccess(std::bind(&FileParser::parseAtomicallyExpression, this));
}


AnyNode FileParser::parseAtomicallyExpression()
{
    // TODO: - breakup
    if (equals(Token::Punctuation, "("))
        return parseBrackets();
    else if (equals(Token::Punctuation, "["))
        return _subParsers.dataType.parse(DataTypeSubParser::Array);
    else if (equals(Token::Punctuation, "{"))
        return _subParsers.block.parseBlock();
    else if (equals(Token::Keyword, "true") || equals(Token::Keyword, "false"))
        return _subParsers.dataType.parse(DataTypeSubParser::Bool);
    else if (equals(Token::Keyword, "while"))
        return _subParsers.loop.parse(LoopSubParser::While);
    else if (equals(Token::Keyword, "do"))
        return _subParsers.loop.parse(LoopSubParser::DoWhile);
    else if (equals(Token::Keyword, "for"))
        return _subParsers.loop.parse(LoopSubParser::For);
    else if (equals(Token::Keyword, "if"))
        return _subParsers.controlFlow.parseIf();
    else if (equals(Token::Keyword, "import"))
        return _subParsers.import.parseImport();
    else if (equals(Token::Keyword, "func")) // Functions should be defined as in C --> will need void type
        return _subParsers.function.parse(FunctionSubParser::Function);
    else if (equals(Token::Keyword, "struct"))
        return _subParsers.classParser.parseStruct();
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
            return _subParsers.variable.parseVariableName();
        case Token::String:
            return _subParsers.dataType.parse(DataTypeSubParser::String);
        case Token::Int:
            return _subParsers.dataType.parse(DataTypeSubParser::Int);
        case Token::Float:
            return _subParsers.dataType.parse(DataTypeSubParser::Float);
        default:
            ThrowException("unexpected token: " + token);
    }
}


void FileParser::skipSemicolonLineEndingIfRequired(const BaseNode &node)
{
    bool doSkipPunctuation = (node.isNodeType<ModuleNode>() || // Bit ugly.
                              node.isNodeType<MathModuleNode>() ||
                              node.isNodeType<IOModuleNode>() ||
                              node.isNodeType<ArrayModuleNode>() ||
                              node.isNodeType<TestModule>() ||
                              node.isNodeType<FileNode>() ||
                              node.isNodeType<ProgramNode>() ||
                              node.isNodeType<IfNode>() ||
                              node.isNodeType<WhileNode>() ||
                              node.isNodeType<DoWhileNode>() ||
                              node.isNodeType<ForLoopNode>() ||
                              node.isNodeType<FunctionNode>());

    if (!doSkipPunctuation)
        skip(";");
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


AnyNode FileParser::parseBrackets()
{
    skip("(");

    AnyNode expression = parseExpression();

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
