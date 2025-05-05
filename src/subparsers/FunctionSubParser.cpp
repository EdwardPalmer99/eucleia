/**
 * @file FunctionSubParser.cpp
 * @author Edward Palmer
 * @date 2025-04-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "FunctionSubParser.hpp"
#include "Exceptions.hpp"
#include "FileParser.hpp"
#include "FunctionCallNode.hpp"
#include "FunctionNode.hpp"
#include "VariableSubParser.hpp"
#include <string>


AnyNode FunctionSubParser::parse(int type, AnyNodeOptional lastExpr)
{
    switch (type)
    {
        case Function:
            return parseFunctionDefinition();
        case FunctionCall:
            return parseFunctionCall(std::move(*lastExpr));
        default:
            ThrowException("unexpected parse type: " + std::to_string(type));
    }
}


FunctionCallNode *FunctionSubParser::parseFunctionCall(AnyNode lastExpression)
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
    auto funcBody = parent().subParsers().block.parseBlock();


    return new FunctionNode(funcName, funcArgs, funcBody);
}