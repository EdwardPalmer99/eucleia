/**
 * @file LoopSubParser.cpp
 * @author Edward Palmer
 * @date 2025-04-21
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "LoopSubParser.hpp"
#include "Exceptions.hpp"
#include "FileParser.hpp"
#include "NodeFactory.hpp"


AnyNode LoopSubParser::parse(int type, AnyNodeOptional lastExpr)
{
    switch (type)
    {
        case DoWhile:
            return parseDoWhile();
        case While:
            return parseWhile();
        case For:
            return parseFor();
        default:
            ThrowException("Invalid parse type: " + std::to_string(type));
    }
}


AnyNode LoopSubParser::parseDoWhile()
{
    skip("do");
    AnyNode body = parent().subParsers().block.parseBlock();
    skip("while");
    AnyNode condition = parent().parseBrackets();

    return NodeFactory::Loops::createDoWhileNode(std::move(body), std::move(condition));
}


AnyNode LoopSubParser::parseWhile()
{
    skip("while");

    AnyNode condition = parent().parseBrackets();
    AnyNode body = parent().subParsers().block.parseBlock();

    return NodeFactory::Loops::createWhileNode(std::move(body), std::move(condition));
}


AnyNode LoopSubParser::parseFor()
{
    skip("for");

    AnyNodeVector forLoopArgs = parseDelimited("(", ")", ";", std::bind(&FileParser::parseExpression, &parent()));
    if (forLoopArgs.size() != 3)
    {
        ThrowException("expected 3 arguments for for-loop but got " + std::to_string(forLoopArgs.size()));
    }

    auto start = forLoopArgs[0];
    auto condition = forLoopArgs[1];
    auto update = forLoopArgs[2];
    auto body = parent().subParsers().block.parseBlock();

    return NodeFactory::Loops::createForLoopNode(std::move(start), std::move(condition), std::move(update), std::move(body));
}