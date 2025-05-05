/**
 * @file UnaryOperatorSubParser.cpp
 * @author Edward Palmer
 * @date 2025-04-26
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "UnaryOperatorSubParser.hpp"
#include "Exceptions.hpp"
#include "FileParser.hpp"
#include "NegationNode.hpp"
#include "NodeFactory.hpp"
#include "NotNode.hpp"
#include "PrefixDecrementNode.hpp"
#include "PrefixIncrementNode.hpp"
#include <string>


AnyNode UnaryOperatorSubParser::parse(int type, AnyNodeOptional lastExpr)
{
    switch (type)
    {
        case Not:
            return parseNot();
        case Negation:
            return parseNegation();
        case PrefixIncrement:
            return parsePrefixIncrement();
        case PrefixDecrement:
            return parsePrefixDecrement();
        default:
            ThrowException("unexpected parse type: " + std::to_string(type));
    }
}


AnyNode UnaryOperatorSubParser::parseNot()
{
    skip("!");

    return NodeFactory::Operators::createNotNode(parent().parseAtomically());
}


AnyNode UnaryOperatorSubParser::parseNegation()
{
    skip("-");

    return NodeFactory::Operators::createNegationNode(parent().parseAtomically());
}


AnyNode UnaryOperatorSubParser::parsePrefixIncrement()
{
    skip("++");

    return new PrefixIncrementNode(parent().parseAtomically()); // TODO:
}


AnyNode UnaryOperatorSubParser::parsePrefixDecrement()
{
    skip("--");

    return new PrefixDecrementNode(parent().parseAtomically()); // TODO:
}