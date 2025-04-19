/**
 * @file ProgramNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "ProgramNode.hpp"
#include "FileParser.hpp"

BaseObject *ProgramNode::evaluate(Scope &scope)
{
    // Create inner program scope for each block of statements. Good example is
    // for a loop where the body of the loop redeclares a variable. This should
    // be okay.
    Scope programScope(scope);

    for (const auto &node : programNodes)
    {
        node->evaluate(programScope);
    }

    // Any memory allocations cleared-up when programScope variable goes out of scope.

    return nullptr; // Nothing returned.
}


BaseNode *ProgramNode::parse(FileParser &parser, bool singleExpr)
{
    parser._skipFunctor("{");

    std::vector<BaseNode *> parsedNodes;

    while (!parser._tokens.empty() && !parser.isPunctuation("}"))
    {
        auto expression = parser.parseExpression();

        parsedNodes.push_back(expression);

        parser.skipSemicolonLineEndingIfRequired(*expression);
    }

    parser._skipFunctor("}");

    if (singleExpr && parsedNodes.size() == 1)
    {
        return parsedNodes.at(0);
    }

    return new ProgramNode(std::move(parsedNodes));
}
