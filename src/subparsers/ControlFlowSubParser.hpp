/**
 * @file ControlFlowParser.hpp
 * @author Edward Palmer
 * @date 2025-04-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "SubParser.hpp"

class AnyNode;
class BreakNode;
class ReturnNode;
class BaseNode;


class ControlFlowSubParser : public SubParser
{
public:
    explicit ControlFlowSubParser(FileParser &parser) : SubParser(parser) {}

    AnyNode *parseIf();

    BreakNode *parseBreak();

    ReturnNode *parseReturn();
};