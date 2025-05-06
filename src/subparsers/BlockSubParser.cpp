/**
 * @file BlockSubParser.cpp
 * @author Edward Palmer
 * @date 2025-04-24
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "BlockSubParser.hpp"
#include "FileParser.hpp"
#include "NodeFactory.hpp"


AnyNode BlockSubParser::parse(int type, AnyNodeOptional lastExpr = std::nullopt)
{
    skip("{");

    AnyNodeVector parsedNodes;

    while (!tokens().empty() && !equals(Token::Punctuation, "}"))
    {
        auto expression = parent().parseExpression();

        parsedNodes.push_back(expression);

        parent().skipSemicolonLineEndingIfRequired(*expression); // TODO: - figure-out
    }

    skip("}");

    if (parsedNodes.size() == 1 && type == BlockOrSingleExpr)
    {
        return parsedNodes.front();
    }

    return NodeFactory::Instances::createArrayNode(std::move(parsedNodes));
}
