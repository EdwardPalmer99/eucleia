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


FunctionCallNode *FunctionSubParser::parseFunctionCall(BaseNode *lastExpressionNode)
{
    auto result = parseFunctionCall(lastExpressionNode->castNode<LookupVariableNode>().name);
    delete lastExpressionNode; /* TODO: - Evil */

    return result;
}


FunctionCallNode *FunctionSubParser::parseFunctionCall(std::string functionName)
{
    auto functionArgs = subparsers().block.parseDelimited("(", ")", ",", std::bind(&FileParser::parseExpression, &parent()));

    return new FunctionCallNode(functionName, functionArgs);
}


FunctionNode *FunctionSubParser::parseFunctionDefinition()
{
    skip("func");

    auto funcName = tokens().dequeue();
    assert(funcName.type() == Token::Variable);

    auto funcArgs = subparsers().block.parseDelimited("(", ")", ",", std::bind(&VariableSubParser::parseVariableDefinition, &subparsers().variable)); // Func variables.
    auto funcBody = parent().subparsers().block.parseBlockLegacy();                                                                                   // TODO: - investigate why this causes a segfault when we switch to parseBlock()

    return new FunctionNode(funcName, funcArgs, funcBody);
}