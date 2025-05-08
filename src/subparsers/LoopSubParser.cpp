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
DoWhileNode *LoopSubParser::parseDoWhile()
{
    skip("do");
    BaseNode *body = parent().subParsers().block.parseBlock();
    skip("while");
    BaseNode *condition = parent().parseBrackets();

    return new DoWhileNode(condition, body);
}


/*
 * Parse:
 * while ([condition is true])
 * {
 * 	[code]
 * }
 */
AnyNode *LoopSubParser::parseWhile()
{
    skip("while");

    BaseNode *condition = parent().parseBrackets();
    BaseNode *body = parent().subParsers().block.parseBlock();

    return NodeFactory::createWhileLoopNode(BaseNode::Ptr(condition), BaseNode::Ptr(body));
}


/*
 * Parse:
 * for ([start]; [condition]; [update])
 * {
 * 	[code]
 * }
 */
AnyNode *LoopSubParser::parseFor()
{
    skip("for");

    ProgramNode *brackets = parseDelimited("(", ")", ";", std::bind(&FileParser::parseExpression, &parent()));

    std::vector<BaseNode *> forLoopArgs = brackets->releaseNodes();

    delete brackets;

    if (forLoopArgs.size() != 3)
    {
        ThrowException("expected 3 arguments for for-loop but got " + std::to_string(brackets->programNodes.size()));
    }

    auto init = forLoopArgs[0];
    auto condition = forLoopArgs[1];
    auto update = forLoopArgs[2];
    auto body = parent().subParsers().block.parseBlock();

    return NodeFactory::createForLoopNode(BaseNode::Ptr(init),
                                          BaseNode::Ptr(condition),
                                          BaseNode::Ptr(update),
                                          BaseNode::Ptr(body));
}