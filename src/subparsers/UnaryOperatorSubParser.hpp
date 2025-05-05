/**
 * @file UnaryOperatorParser.hpp
 * @author Edward Palmer
 * @date 2025-04-26
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "SubParser.hpp"

class FileParser;
class NotNode;
class NegationNode;
class PrefixIncrementNode;
class PrefixDecrementNode;

// TODO: - have a ParseEnum Type that we use then just call "Parse" method with type or int

class UnaryOperatorSubParser : public SubParser
{
public:
    explicit UnaryOperatorSubParser(FileParser &parser) : SubParser(parser) {}

    /* Parse ![value] */
    NotNode *parseNot();

    /* Parse -[value] */
    NegationNode *parseNegation();

    /* Parse ++[value] */
    PrefixIncrementNode *parsePrefixIncrement();

    /* Parse --[value] */
    PrefixDecrementNode *parsePrefixDecrement();
};