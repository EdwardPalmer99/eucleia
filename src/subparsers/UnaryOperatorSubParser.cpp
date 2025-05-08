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
#include "NegationNode.hpp"
#include "NodeFactory.hpp"
#include "PrefixDecrementNode.hpp"
#include "PrefixIncrementNode.hpp"


AnyNode *UnaryOperatorSubParser::parseNot()
{
    skip("!");

    return NodeFactory::createNotNode(BaseNode::Ptr(parent().parseAtomically()));
}


NegationNode *UnaryOperatorSubParser::parseNegation()
{
    skip("-");

    return new NegationNode(parent().parseAtomically());
}


PrefixIncrementNode *UnaryOperatorSubParser::parsePrefixIncrement()
{
    skip("++");

    return new PrefixIncrementNode(parent().parseAtomically());
}


PrefixDecrementNode *UnaryOperatorSubParser::parsePrefixDecrement()
{
    skip("--");

    return new PrefixDecrementNode(parent().parseAtomically());
}