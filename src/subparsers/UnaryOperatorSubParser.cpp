/**
 * @file UnaryOperatorSubParser.cpp
 * @author Edward Palmer
 * @date 2025-04-26
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "UnaryOperatorSubParser.hpp"
#include "FileParser.hpp"
#include "NegationNode.hpp"
#include "NotNode.hpp"
#include "PrefixDecrementNode.hpp"
#include "PrefixIncrementNode.hpp"


NotNode *UnaryOperatorSubParser::parseNot()
{
    _parser.skip("!");

    return new NotNode(_parser.parseAtomically());
}


NegationNode *UnaryOperatorSubParser::parseNegation()
{
    _parser.skip("-");

    return new NegationNode(_parser.parseAtomically());
}


PrefixIncrementNode *UnaryOperatorSubParser::parsePrefixIncrement()
{
    _parser.skip("++");

    return new PrefixIncrementNode(_parser.parseAtomically());
}


PrefixDecrementNode *UnaryOperatorSubParser::parsePrefixDecrement()
{
    _parser.skip("--");

    return new PrefixDecrementNode(_parser.parseAtomically());
}