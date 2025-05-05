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
    auto functionArgs = _parser.parseDelimited("(", ")", ",", std::bind(&FileParser::parseExpression, &_parser));

    return new FunctionCallNode(functionName, functionArgs);
}


FunctionNode *FunctionSubParser::parseFunctionDefinition()
{
    _parser.skip("func");

    auto funcName = _parser.parseVariableName();
    auto funcArgs = _parser.parseDelimited("(", ")", ",", std::bind(&FileParser::parseVariableDefinition, &_parser)); // Func variables.
    auto funcBody = _parser._blockParser.parseBlock();

    return new FunctionNode(funcName, funcArgs, funcBody);
}