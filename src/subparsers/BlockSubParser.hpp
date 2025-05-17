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
#include "BaseNode.hpp"
#include "SubParser.hpp"
#include <string>
#include <vector>

class FileParser;
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

    /* Parse expressions inside { } and wrap-inside AnyNode::Ptr */
    AnyNode::Ptr parseBlock();

    /* Parse expressions inside {} and return vector of shared pointers */
    BaseNodePtrVector parseBraces();

    /* Parse a delimited expression, e.g. (a, b, c) where start='(', stop=')', separator=',' */
    BaseNodePtrVector parseDelimited(std::string start,
                                     std::string stop,
                                     std::string separator,
                                     ParseMethod parseMethod);
};