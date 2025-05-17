/**
 * @file LoopSubParser.cpp
 * @author Edward Palmer
 * @date 2025-04-21
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "LoopSubParser.hpp"
#include "AnyNode.hpp"
#include "FileParser.hpp"
#include "NodeFactory.hpp"

/*
 * Parse:
 * do
 * {
 * 	[code]
 * }
 * while ([condition is true]);
 */
AnyNode::Ptr LoopSubParser::parseDoWhile()
{
    skip("do");
    BaseNode::Ptr body = parent().subparsers().block.parseBlock();
    skip("while");
    BaseNode::Ptr condition = parent().parseBrackets();

    return NodeFactory::createDoWhileLoopNode(condition, body);
}


/*
 * Parse:
 * while ([condition is true])
 * {
 * 	[code]
 * }
 */
AnyNode::Ptr LoopSubParser::parseWhile()
{
    skip("while");

    BaseNode::Ptr condition = parent().parseBrackets();
    BaseNode::Ptr body = parent().subparsers().block.parseBlock();

    return NodeFactory::createWhileLoopNode(condition, body);
}


/*
 * Parse:
 * for ([start]; [condition]; [update])
 * {
 * 	[code]
 * }
 */
AnyNode::Ptr LoopSubParser::parseFor()
{
    skip("for");

    auto forLoopArgs = subparsers().block.parseDelimited("(", ")", ";", std::bind(&FileParser::parseExpression, &parent()));

    if (forLoopArgs.size() != 3)
    {
        ThrowException("expected 3 arguments for for-loop but got " + std::to_string(forLoopArgs.size()));
    }

    auto init = forLoopArgs[0];
    auto condition = forLoopArgs[1];
    auto update = forLoopArgs[2];
    auto body = subparsers().block.parseBlock();

    return NodeFactory::createForLoopNode(init,
                                          condition,
                                          update,
                                          body);
}