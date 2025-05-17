/**
 * @file FunctionSubParser.cpp
 * @author Edward Palmer
 * @date 2025-04-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "FunctionSubParser.hpp"
#include "FileParser.hpp"
#include "FunctionCallNode.hpp"
#include "FunctionNode.hpp"
#include "VariableSubParser.hpp"
#include <cassert>


FunctionCallNode::Ptr FunctionSubParser::parseFunctionCall(BaseNode::Ptr lastExpressionNode)
{
    return parseFunctionCall(lastExpressionNode->castNode<LookupVariableNode>().name);
}


FunctionCallNode::Ptr FunctionSubParser::parseFunctionCall(std::string functionName)
{
    auto functionArgs = subparsers().block.parseDelimited("(", ")", ",", std::bind(&FileParser::parseExpression, &parent()));

    return std::make_shared<FunctionCallNode>(functionName, functionArgs);
}


FunctionNode::Ptr FunctionSubParser::parseFunctionDefinition()
{
    skip("func");

    auto funcName = tokens().dequeue();
    assert(funcName.type() == Token::Variable);

    auto funcArgs = subparsers().block.parseDelimited("(", ")", ",", std::bind(&VariableSubParser::parseVariableDefinition, &subparsers().variable)); // Func variables.
    auto funcBody = parent().subparsers().block.parseBlock();                                                                                         // TODO: - investigate why this causes a segfault when we switch to parseBlock()

    return std::make_shared<FunctionNode>(funcName, funcArgs, funcBody);
}