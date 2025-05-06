/**
 * @file BlockSubParser.hpp
 * @author Edward Palmer
 * @date 2025-04-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "AnyNode.hpp"
#include "SubParser.hpp"

class FileParser;
class ProgramNode;
class BaseNode;

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

    enum Type
    {
        Block,
        BlockOrSingleExpr /* If we have a single expression within a block, return that */
    };

    AnyNode parse(int type, AnyNodeOptional lastExpr = std::nullopt) override;
};