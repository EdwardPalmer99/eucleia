/**
 * @file DoWhileNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "DoWhileNode.hpp"
#include "Exceptions.hpp"
#include "ExpressionScope.hpp"
#include "FileParser.hpp"
#include "IntObject.hpp"
#include "JumpPoints.hpp"
#include "ProgramNode.hpp"

BaseObject *DoWhileNode::evaluate(Scope &scope)
{
    jmp_buf local;
    pushBreakJumpPoint(&local);

    if (setjmp(local) != 1)
    {
        Scope loopScope(scope); // Extend scope.

        do
        {
            (void)body->evaluate(loopScope);
        } while (evaluateExpression<BoolObject::Type>(condition, scope));
    }

    // Restore original context.
    popBreakJumpPoint();

    return nullptr; // Return nothing.
}


DoWhileNode *DoWhileNode::parse(FileParser &parser)
{
    parser._skipFunctor("do");
    BaseNode *body = ProgramNode::parse(parser, true);
    parser._skipFunctor("while");
    BaseNode *condition = parser.parseBrackets();

    return new DoWhileNode(condition, body);
}
