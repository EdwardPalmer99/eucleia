/**
 * @file UnaryOperatorParser.hpp
 * @author Edward Palmer
 * @date 2025-04-26
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "AnyNode.hpp"
#include "SubParser.hpp"

class FileParser;

// TODO: - have a ParseEnum Type that we use then just call "Parse" method with type or int
// TODO: - Can have single type of UnaryNode and then just an enum with the type of that since
// all do pretty-much the same operation

class UnaryOperatorSubParser : public SubParser
{
public:
    explicit UnaryOperatorSubParser(FileParser &parser) : SubParser(parser) {}

    /* Parse ![value] */
    AnyNode::Ptr parseNot();

    /* Parse -[value] */
    AnyNode::Ptr parseNegation();

    /* Parse ++[value] */
    AnyNode::Ptr parsePrefixIncrement();

    /* Parse --[value] */
    AnyNode::Ptr parsePrefixDecrement();
};