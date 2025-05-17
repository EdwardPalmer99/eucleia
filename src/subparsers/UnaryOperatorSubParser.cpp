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

AnyNode::Ptr UnaryOperatorSubParser::parseNot()
{
    skip("!");

    return NodeFactory::createNotNode(parent().parseAtomically());
}


AnyNode::Ptr UnaryOperatorSubParser::parseNegation()
{
    skip("-");

    return NodeFactory::createNegationNode(parent().parseAtomically());
}


AnyNode::Ptr UnaryOperatorSubParser::parsePrefixIncrement()
{
    skip("++");

    return NodeFactory::createPrefixIncrementNode(parent().parseAtomically());
}


AnyNode::Ptr UnaryOperatorSubParser::parsePrefixDecrement()
{
    skip("--");

    return NodeFactory::createPrefixDecrementNode(parent().parseAtomically());
}