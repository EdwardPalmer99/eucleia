/**
 * @file LoopParser.cpp
 * @author Edward Palmer
 * @date 2025-04-21
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "LoopParser.hpp"
#include "FileParser.hpp"


/*
 * Parse:
 * do
 * {
 * 	[code]
 * }
 * while ([condition is true]);
 */
DoWhileNode *LoopParser::parseDoWhile()
{
    _parser.skip("do");
    BaseNode *body = _parser.parseProgram();
    _parser.skip("while");
    BaseNode *condition = _parser.parseBrackets();

    return new DoWhileNode(condition, body);
}


/*
 * Parse:
 * while ([condition is true])
 * {
 * 	[code]
 * }
 */
WhileNode *LoopParser::parseWhile()
{
    _parser.skip("while");

    BaseNode *condition = _parser.parseBrackets();
    BaseNode *body = _parser.parseProgram();

    return new WhileNode(condition, body);
}


/*
 * Parse:
 * for ([start]; [condition]; [update])
 * {
 * 	[code]
 * }
 */
ForLoopNode *LoopParser::parseFor()
{
    _parser.skip("for");

    ProgramNode *brackets = _parser.parseDelimited("(", ")", ";", std::bind(&FileParser::parseExpression, &_parser));

    std::vector<BaseNode *> forLoopArgs = brackets->releaseNodes();

    delete brackets;

    if (forLoopArgs.size() != 3)
    {
        ThrowException("expected 3 arguments for for-loop but got " + std::to_string(brackets->programNodes.size()));
    }

    auto start = forLoopArgs[0];
    auto condition = forLoopArgs[1];
    auto update = forLoopArgs[2];
    auto body = _parser.parseProgram();

    return new ForLoopNode(start, condition, update, body);
}