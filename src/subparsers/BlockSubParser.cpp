/**
 * @file BlockSubParser.cpp
 * @author Edward Palmer
 * @date 2025-04-24
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "BlockSubParser.hpp"
#include "BaseNode.hpp"
#include "FileParser.hpp"
#include "ProgramNode.hpp"

BaseNode *BlockSubParser::parseBlock(bool extractSingleExpr)
{
    skip("{");

    std::vector<BaseNode *> parsedNodes;

    while (!tokens().empty() && !equals(Token::Punctuation, "}"))
    {
        auto expression = parent().parseExpression();

        parsedNodes.push_back(expression);

        parent().skipSemicolonLineEndingIfRequired(*expression);
    }

    skip("}");

    if (extractSingleExpr && parsedNodes.size() == 1)
    {
        return parsedNodes.front();
    }

    return new ProgramNode(std::move(parsedNodes));
}
