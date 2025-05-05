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
#include "Tokenizer.hpp"
#include "WhileNode.hpp"

/* Forwards declaration of complete parser */
class FileParser;


class LoopParser
{
public:
    LoopParser() = delete;
    explicit LoopParser(FileParser &parser) : _parser(parser) {}

    DoWhileNode *parseDoWhile();

    WhileNode *parseWhile();

    ForLoopNode *parseFor();

private:
    /* Store a reference to the complete-parser */
    FileParser &_parser;
};
