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


FunctionCallNode *FunctionSubParser::parseFunctionCall(BaseNode *lastExpression)
{
    auto functionName = std::move(lastExpression);
    auto functionArgs = parseDelimited("(", ")", ",", std::bind(&FileParser::parseExpression, &parent()));

    return new FunctionCallNode(functionName, functionArgs);
}


FunctionNode *FunctionSubParser::parseFunctionDefinition()
{
    skip("func");

    auto funcName = parent().subParsers().variable.parseVariableName();
    auto funcArgs = parseDelimited("(", ")", ",", std::bind(&VariableSubParser::parseVariableDefinition, &parent().subParsers().variable)); // Func variables.
    auto funcBody = parent().subParsers().block.parseBlockLegacy();                                                                         // TODO: - investigate why this causes a segfault when we switch to parseBlock()

    return new FunctionNode(funcName, funcArgs, funcBody);
}