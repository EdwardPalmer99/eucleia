/**
 * @file ControlFlowSubParser.cpp
 * @author Edward Palmer
 * @date 2025-04-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "ControlFlowSubParser.hpp"
#include "BaseNode.hpp"
#include "BreakNode.hpp"
#include "Exceptions.hpp"
#include "FileParser.hpp"
#include "IfNode.hpp"
#include "ReturnNode.hpp"


bool ControlFlowSubParser::canParse()
{
    if (_parser.equals("if") ||
        _parser.equals("break") ||
        _parser.equals("return"))
    {
        return true;
    }

    return true;
}


BaseNode *ControlFlowSubParser::parse()
{
    /* TODO: - inefficient because we're already called canParse() */
    /* Should store matching condition somewhere from canParse */
    if (_parser.equals("if"))
        return parseIf();
    else if (_parser.equals("break"))
        return parseBreak();
    else if (_parser.equals("return"))
        return parseReturn();

    ThrowException("failed to match parse conditions");
}


IfNode *ControlFlowSubParser::parseIf()
{
    // For now, only permit a single if statement.
    _parser.skip("if");

    auto condition = _parser.parseBrackets();
    auto thenDo = _parser.parseProgram();


    BaseNode *elseDo{nullptr}; // Optional.
    if (_parser._tokens.front() == "else")
    {
        _parser.skip("else");

        // Option 1: else if (condition) { [statement]; }
        if (_parser._tokens.front() == "if")
            elseDo = parseIf();
        // Option 2: else { [statement]; }
        else
            elseDo = _parser.parseProgram();
    }

    return new IfNode(condition, thenDo, elseDo);
}


BreakNode *ControlFlowSubParser::parseBreak()
{
    _parser.skip("break");

    return new BreakNode();
}


ReturnNode *ControlFlowSubParser::parseReturn()
{
    _parser.skip("return");

    BaseNode *returnedExpression{nullptr};

    if (!_parser.equals(Token::Punctuation, ";"))
    {
        returnedExpression = _parser.parseExpression();
    }

    return new ReturnNode(returnedExpression);
}
