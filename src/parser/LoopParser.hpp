/**
 * @file LoopParser.hpp
 * @author Edward Palmer
 * @date 2025-04-21
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "BaseParser.hpp"
#include "DoWhileNode.hpp"
#include "ForLoopNode.hpp"
#include "SubParser.hpp"
#include "Tokenizer.hpp"
#include "WhileNode.hpp"

class LoopParser : public SubParser
{
public:
    explicit LoopParser(FileParser &parser) : SubParser(parser) {}

    DoWhileNode *parseDoWhile();

    WhileNode *parseWhile();

    ForLoopNode *parseFor();
};
