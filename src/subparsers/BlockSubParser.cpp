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
#include "NodeFactory.hpp"
#include "ProgramNode.hpp"


AnyNode *BlockSubParser::parseBlock()
{
    // TODO: - if there is only a single expression inside then return that

    BaseNodePtrVector capturedNodes;

    for (auto *node : parseBraces()) /* TODO: - not ideal, wrapper to convert raw -> shared-ptr */
    {
        capturedNodes.emplace_back(node);
    }

    return NodeFactory::createBlockNode(capturedNodes);
}


BaseNode *BlockSubParser::parseBlockLegacy()
{
    // TODO: - remove
    auto capturedNodes = parseBraces();

    if (capturedNodes.size() == 1)
    {
        return capturedNodes.front();
    }

    return new ProgramNode(std::move(capturedNodes));
}


std::vector<BaseNode *> BlockSubParser::parseBraces()
{
    skip("{");

    std::vector<BaseNode *> capturedNodes; /* Nodes inside the block */

    while (!tokens().empty() && !equals(Token::Punctuation, "}"))
    {
        auto expression = parent().parseExpression();

        capturedNodes.push_back(expression); /* TODO: - switch to shared pointer here */

        parent().skipSemicolonLineEndingIfRequired(*expression);
    }

    skip("}");

    return capturedNodes;
}
