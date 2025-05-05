/**
 * @file SubParser.hpp
 * @author Edward Palmer
 * @date 2025-04-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "BaseParser.hpp"

class BaseNode;
class FileParser;

// TODO: - inherit from BaseParser with initialization method so don't need to call parent parser methods

/* Subparsers build a complete parser (composition) */
class SubParser : public BaseParser
{
public:
    explicit SubParser(FileParser &parser_);

    /* Returns reference to parent parser for file */
    [[nodiscard]] FileParser &parent() { return _parser; }

    /* Returns reference to file tokens */
    [[nodiscard]] Tokens &tokens() final;

private:
    /* Store a reference to the parent parser for the file */
    FileParser &_parser;
};


/* TODO: - SubParsers should be incredibly light-weight. Should we just not store any and then
 * create one as we need it (keeps a reference to the tokens and that's it? )
 * Also need something to store the last parsed value etc
 */