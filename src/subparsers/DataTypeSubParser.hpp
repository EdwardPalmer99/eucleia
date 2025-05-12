/**
 * @file DataTypeSubParser.hpp
 * @author Edward Palmer
 * @date 2025-04-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "SubParser.hpp"

class FileParser;
class ArrayAccessNode;
class AnyNode;
class AnyPropertyNode;


class DataTypeSubParser : public SubParser
{
public:
    explicit DataTypeSubParser(FileParser &parser) : SubParser(parser) {}

    AnyNode *parseInt();

    AnyNode *parseFloat();

    /* Parse: true/false */
    AnyNode *parseBool();

    /* Parse: "..." */
    AnyNode *parseString();

    /* Parse: [1, 2, 3, 4] OR [true, false, true] OR [1.2, 2.4] OR ["hello, ", "world!"] */
    AnyNode *parseArray();

    /* Parse: array_variable_name[index] */
    AnyPropertyNode *parseArrayAccessor(BaseNode *lastExpression);
};