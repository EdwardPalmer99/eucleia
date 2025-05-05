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
    _parser.skip("{");

    std::vector<BaseNode *> parsedNodes;

    while (!_parser._tokens.empty() && !_parser.equals(Token::Punctuation, "}"))
    {
        auto expression = _parser.parseExpression();

        parsedNodes.push_back(expression);

        _parser.skipSemicolonLineEndingIfRequired(*expression);
    }

    _parser.skip("}");

    if (extractSingleExpr && parsedNodes.size() == 1)
    {
        return parsedNodes.front();
    }

    return new ProgramNode(std::move(parsedNodes));
}
