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

    BaseNodeSharedPtrVector capturedNodes;

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


BaseNodePtrVector BlockSubParser::parseBraces()
{
    skip("{");

    BaseNodePtrVector capturedNodes; /* Nodes inside the block */

    while (!tokens().empty() && !equals(Token::Punctuation, "}"))
    {
        auto expression = parent().parseExpression();

        capturedNodes.push_back(expression); /* TODO: - switch to shared pointer here */

        parent().skipSemicolonLineEndingIfRequired(*expression);
    }

    skip("}");

    return capturedNodes;
}


BaseNodePtrVector BlockSubParser::parseDelimited(std::string start,
                                                 std::string stop,
                                                 std::string separator,
                                                 ParseMethod parseMethod)
{
    skip(start);

    BaseNodePtrVector capturedNodes;

    // Iterate while we still have tokens and haven't reached stop token.
    bool firstCall = true;

    while (!tokens().empty() && !equals(stop))
    {
        /* Skip separator on each subsequent call (i.e. a, b) */
        if (firstCall)
        {
            firstCall = false;
        }
        else
        {
            skip(separator);
        }

        if (equals(stop))
        {
            break;
        }

        // Parse the token.
        capturedNodes.push_back(parseMethod());
    }

    skip(stop);

    capturedNodes.shrink_to_fit();
    return capturedNodes;
}