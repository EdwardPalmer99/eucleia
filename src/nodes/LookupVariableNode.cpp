/**
 * @file LookupVariableNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "LookupVariableNode.hpp"
#include "FileParser.hpp"

BaseObject *LookupVariableNode::evaluate(Scope &scope)
{
    return scope.getNamedObject(name);
}

LookupVariableNode *LookupVariableNode::parse(FileParser &parser)
{
    return new LookupVariableNode(parser.tokens().dequeue().validated(Token::Variable));
}
