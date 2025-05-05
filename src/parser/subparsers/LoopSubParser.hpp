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
#include "DoWhileNode.hpp"
#include "ForLoopNode.hpp"
#include "SubParser.hpp"
#include "Tokenizer.hpp"
#include "WhileNode.hpp"

class LoopSubParser : public SubParser
{
public:
    explicit LoopSubParser(FileParser &parser) : SubParser(parser) {}

    /* TODO: - */
    bool canParse() override { ThrowException("not implemented"); }

    BaseNode *parse() override { ThrowException("not implemented"); }

    DoWhileNode *parseDoWhile();

    WhileNode *parseWhile();

    ForLoopNode *parseFor();
};
