/**
 * @file LoopSubParser.hpp
 * @author Edward Palmer
 * @date 2025-04-21
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "AnyNode.hpp"
#include "BaseParser.hpp"
#include "SubParser.hpp"
#include "Tokenizer.hpp"


class LoopSubParser : public SubParser
{
public:
    explicit LoopSubParser(FileParser &parser) : SubParser(parser) {}

    AnyNode::Ptr parseDoWhile();

    AnyNode::Ptr parseWhile();

    AnyNode::Ptr parseFor();
};
