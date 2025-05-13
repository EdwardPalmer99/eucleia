/**
 * @file FunctionSubParser.hpp
 * @author Edward Palmer
 * @date 2025-04-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "SubParser.hpp"
#include <string>


class FileParser;
class FunctionNode;
class FunctionCallNode;

/* TODO: - parser should keep track of previously parsed expressions by storing in a vector so we can use */


class FunctionSubParser : public SubParser
{
public:
    explicit FunctionSubParser(FileParser &parser) : SubParser(parser) {}

    /* Parse function definition:
     *
     * func [funcName]([dataType1] [arg1], ...)
     * {
     *      [code]
     * }
     *
     */
    FunctionNode *parseFunctionDefinition();

    /*
     * Parse function call: [funcName]([funcArgs])
     * Example: test(a, b, c);
     */
    FunctionCallNode *parseFunctionCall(BaseNode *lastExpressionNode);

    FunctionCallNode *parseFunctionCall(std::string funcName);
};