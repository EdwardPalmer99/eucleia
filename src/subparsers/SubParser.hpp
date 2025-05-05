/**
 * @file SubParser.hpp
 * @author Edward Palmer
 * @date 2025-04-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

class FileParser;
class BaseNode;


/* Subparsers build a complete parser (composition) */
class SubParser
{
public:
    SubParser() = delete;
    explicit SubParser(FileParser &parser) : _parser(parser) {}

    /* Returns true if subparser can parse token */
    // virtual bool canParse() = 0;

    /* Parse expression */
    // virtual BaseNode *parse() = 0;

protected:
    /* Store a reference to the complete-parser */
    FileParser &_parser;
};