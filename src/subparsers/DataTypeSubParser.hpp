/**
 * @file DataTypeSubParser.hpp
 * @author Edward Palmer
 * @date 2025-04-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "AnyNode.hpp"
#include "SubParser.hpp"

class FileParser;
class ArrayAccessNode;

class DataTypeSubParser : public SubParser
{
public:
    explicit DataTypeSubParser(FileParser &parser) : SubParser(parser) {}

    AnyNode::Ptr parseInt();

    AnyNode::Ptr parseFloat();

    /* Parse: true/false */
    AnyNode::Ptr parseBool();

    /* Parse: "..." */
    AnyNode::Ptr parseString();

    /* Parse: [1, 2, 3, 4] OR [true, false, true] OR [1.2, 2.4] OR ["hello, ", "world!"] */
    AnyNode::Ptr parseArray();

    /* Parse: array_variable_name[index] */
    AnyPropertyNode::Ptr parseArrayAccessor(BaseNode::Ptr lastExpression);
};