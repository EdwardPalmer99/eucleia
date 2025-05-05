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


FunctionCallNode *FunctionSubParser::parseFunctionCall(BaseNode *lastExpression)
{
    auto functionName = std::move(lastExpression);
    auto functionArgs = parent().parseDelimited("(", ")", ",", std::bind(&FileParser::parseExpression, &parent()));

    return new FunctionCallNode(functionName, functionArgs);
}


FunctionNode *FunctionSubParser::parseFunctionDefinition()
{
    skip("func");

    auto funcName = parent().parseVariableName();
    auto funcArgs = parent().parseDelimited("(", ")", ",", std::bind(&FileParser::parseVariableDefinition, &parent())); // Func variables.
    auto funcBody = parent().subParsers().block.parseBlock();

    return new FunctionNode(funcName, funcArgs, funcBody);
}