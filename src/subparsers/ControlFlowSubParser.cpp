/**
 * @file ControlFlowSubParser.cpp
 * @author Edward Palmer
 * @date 2025-04-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "ControlFlowSubParser.hpp"
#include "AnyNode.hpp"
#include "BaseNode.hpp"
#include "Exceptions.hpp"
#include "FileParser.hpp"
#include "NodeFactory.hpp"

// bool ControlFlowSubParser::canParse()
// {
//     if (equals("if") ||
//         equals("break") ||
//         equals("return"))
//     {
//         return true;
//     }

//     return true;
// }


// BaseNode::Ptr ControlFlowSubParser::parse()
// {
//     /* TODO: - inefficient because we're already called canParse() */
//     /* Should store matching condition somewhere from canParse */
//     if (equals("if"))
//         return parseIf();
//     else if (equals("break"))
//         return parseBreak();
//     else if (equals("return"))
//         return parseReturn();

//     ThrowException("failed to match parse conditions");
// }


AnyNode::Ptr ControlFlowSubParser::parseIf()
{
    // For now, only permit a single if statement.
    skip("if");

    auto condition = parent().parseBrackets();
    auto thenDo = parent().subparsers().block.parseBlock();

    BaseNode::Ptr elseDo{nullptr}; // Optional.
    if (tokens().front() == "else")
    {
        skip("else");

        // Option 1: else if (condition) { [statement]; }
        if (tokens().front() == "if")
            elseDo = parseIf();
        // Option 2: else { [statement]; }
        else
            elseDo = parent().subparsers().block.parseBlock();
    }

    return NodeFactory::createIfNode(condition, thenDo, elseDo);
}


AnyNode::Ptr ControlFlowSubParser::parseBreak()
{
    skip("break");

    return NodeFactory::createBreakNode();
}


AnyNode::Ptr ControlFlowSubParser::parseReturn()
{
    skip("return");

    BaseNode::Ptr returnedExpression{nullptr};

    if (!equals(Token::Punctuation, ";"))
    {
        returnedExpression = parent().parseExpression();
    }

    return NodeFactory::createReturnNode(returnedExpression);
}
