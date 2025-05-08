/**
 * @file LoopSubParser.hpp
 * @author Edward Palmer
 * @date 2025-04-21
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "BaseParser.hpp"
#include "SubParser.hpp"
#include "Tokenizer.hpp"

class AnyNode;

class LoopSubParser : public SubParser
{
public:
    explicit LoopSubParser(FileParser &parser) : SubParser(parser) {}

    AnyNode *parseDoWhile();

    AnyNode *parseWhile();

    AnyNode *parseFor();
};
