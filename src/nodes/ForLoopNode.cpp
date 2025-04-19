/**
 * @file ForLoopNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "ForLoopNode.hpp"
#include "ExpressionScope.hpp"
#include "FileParser.hpp"
#include "IntObject.hpp"
#include "JumpPoints.hpp"

BaseObject *ForLoopNode::evaluate(Scope &scope)
{
    // Initialization.
    Scope loopScope(scope); // Extend scope.

    (void)start->evaluate(loopScope);

    jmp_buf local;
    pushBreakJumpPoint(&local);

    // Add evaluation to forScope:
    if (setjmp(local) != 1)
    {
        for (;
             evaluateExpression<BoolObject::Type>(condition, loopScope); // TODO: - not very efficient repeatedly recalculating...
             update->evaluate(loopScope))
        {
            (void)body->evaluate(loopScope);
        }
    }

    popBreakJumpPoint();

    return nullptr;
}


ForLoopNode *ForLoopNode::parse(FileParser &parser)
{
    parser._skipFunctor("for");

    ProgramNode *brackets = parser.parseDelimited("(", ")", ";", std::bind(&FileParser::parseExpression, std::placeholders::_1));

    std::vector<BaseNode *> forLoopArgs = brackets->releaseNodes();

    delete brackets;

    if (forLoopArgs.size() != 3)
    {
        ThrowException("expected 3 arguments for for-loop but got " + std::to_string(brackets->programNodes.size()));
    }

    auto start = forLoopArgs[0];
    auto condition = forLoopArgs[1];
    auto update = forLoopArgs[2];
    auto body = ProgramNode::parse(parser, true);

    return new ForLoopNode(start, condition, update, body);
}