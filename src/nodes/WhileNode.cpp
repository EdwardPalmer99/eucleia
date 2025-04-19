/**
 * @file WhileNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "WhileNode.hpp"
#include "ExpressionScope.hpp"
#include "FileNode.hpp"
#include "FileParser.hpp"
#include "JumpPoints.hpp"
#include "ProgramNode.hpp"

BaseObject *WhileNode::evaluate(Scope &scope)
{
    // Set jump point for break statements.
    jmp_buf local;
    pushBreakJumpPoint(&local);

    if (setjmp(local) != 1)
    {
        Scope loopScope(scope); // Extend scope.

        while (evaluateExpression<BoolObject::Type>(condition, scope))
        {
            (void)body->evaluate(loopScope);
        }
    }

    // Restore original context.
    popBreakJumpPoint();

    return nullptr;
}


WhileNode *WhileNode::parse(FileParser &parser)
{
    parser._skipFunctor("while");

    BaseNode *condition = parser.parseBrackets();
    BaseNode *body = ProgramNode::parse(parser, true);

    return new WhileNode(condition, body);
}
