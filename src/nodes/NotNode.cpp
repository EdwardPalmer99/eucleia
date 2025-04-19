/**
 * @file NotNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "NotNode.hpp"
#include "FileParser.hpp"
#include "IntObject.hpp"

BaseObject *NotNode::evaluate(Scope &scope)
{
    auto bodyEvaluated = body->evaluate<BoolObject>(scope);

    return scope.createManagedObject<BoolObject>(!(*bodyEvaluated));
}


NotNode *NotNode::parse(FileParser &parser)
{
    parser._skipFunctor("!");
    return new NotNode(parser.parseAtomically());
}
