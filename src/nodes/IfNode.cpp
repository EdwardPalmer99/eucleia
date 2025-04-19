/**
 * @file IfNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "IfNode.hpp"
#include "ExpressionScope.hpp"
#include "FileParser.hpp"
#include "IntObject.hpp"
#include "ProgramNode.hpp"

BaseObject *IfNode::evaluate(Scope &scope)
{
    // NB: the condition should be evaluated in its own scope as it is within ()
    // brackets. We don't want it to persist once evaluated.
    if (evaluateExpression<BoolObject::Type>(ifCondition, scope))
        return thenDo->evaluate(scope);
    else if (elseDo != nullptr) // Optional "else"
        return elseDo->evaluate(scope);
    else
        return nullptr;
}


IfNode *IfNode::parse(FileParser &parser)
{
    // For now, only permit a single if statement.
    parser._skipFunctor("if");

    auto condition = parser.parseBrackets();
    auto thenDo = ProgramNode::parse(parser, true);

    BaseNode *elseDo{nullptr}; // Optional.
    if (parser.tokens().front() == "else")
    {
        parser._skipFunctor("else");

        // Option 1: else if (condition) { [statement]; }
        if (parser.tokens().front() == "if")
            elseDo = IfNode::parse(parser);
        // Option 2: else { [statement]; }
        else
            elseDo = ProgramNode::parse(parser, true);
    }

    return new IfNode(condition, thenDo, elseDo);
}
