/**
 * @file FunctionSubParser.hpp
 * @author Edward Palmer
 * @date 2025-04-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "AnyNode.hpp"
#include "SubParser.hpp"

class FileParser;
class FunctionNode;
class FunctionCallNode;

/* TODO: - parser should keep track of previously parsed expressions by storing in a vector so we can use */


class FunctionSubParser : public SubParser
{
public:
    explicit FunctionSubParser(FileParser &parser) : SubParser(parser) {}

    enum Type
    {
        Function,
        FunctionCall
    };

    AnyNode parse(int type, AnyNodeOptional lastExpr) override;

protected:
    /* Parse function definition:
     *
     * func [funcName]([dataType1] [arg1], ...)
     * {
     *      [code]
     * }
     *
     */
    AnyNode parseFunctionDefinition();

    /*
     * Parse function call: [funcName]([funcArgs])
     * Example: test(a, b, c);
     */
    AnyNode parseFunctionCall(AnyNode lastExpression);
};