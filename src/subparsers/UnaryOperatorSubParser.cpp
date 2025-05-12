/**
 * @file UnaryOperatorSubParser.cpp
 * @author Edward Palmer
 * @date 2025-04-26
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "UnaryOperatorSubParser.hpp"
#include "AnyNode.hpp"
#include "FileParser.hpp"
#include "NodeFactory.hpp"

AnyNode *UnaryOperatorSubParser::parseNot()
{
    skip("!");

    return NodeFactory::createNotNode(BaseNode::Ptr(parent().parseAtomically()));
}


AnyNode *UnaryOperatorSubParser::parseNegation()
{
    skip("-");

    return NodeFactory::createNegationNode(parent().parseAtomically());
}


AnyNode *UnaryOperatorSubParser::parsePrefixIncrement()
{
    skip("++");

    return NodeFactory::createPrefixIncrementNode(parent().parseAtomically());
}


AnyNode *UnaryOperatorSubParser::parsePrefixDecrement()
{
    skip("--");

    return NodeFactory::createPrefixDecrementNode(parent().parseAtomically());
}