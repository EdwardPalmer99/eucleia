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
#include "AnyNode.hpp"

class ControlFlowSubParser : public SubParser
{
public:
    explicit ControlFlowSubParser(FileParser &parser) : SubParser(parser) {}

    AnyNode::Ptr parseIf();

    AnyNode::Ptr parseBreak();

    AnyNode::Ptr parseReturn();
};