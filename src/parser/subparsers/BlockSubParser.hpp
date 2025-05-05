/**
 * @file BlockSubParser.hpp
 * @author Edward Palmer
 * @date 2025-04-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "SubParser.hpp"

class ProgramNode;

/*
 * This subparser is used for parsing a "block" of code:
 * example:
 * {
 *  line 1;
 *  line 2;
 * ...
 * }
 */
class BlockSubParser : public SubParser
{
public:
    explicit BlockSubParser(FileParser &parser) : SubParser(parser) {}

    /* Option: return single expression OR always wrap-up in block */
    ProgramNode *parseBlock();


}; // TODO: - finish implementing