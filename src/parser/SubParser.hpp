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

/* Subparsers build a complete parser (composition) */
class SubParser
{
public:
    SubParser() = delete;
    explicit SubParser(FileParser &parser) : _parser(parser) {}

protected:
    /* Store a reference to the complete-parser */
    FileParser &_parser;
};