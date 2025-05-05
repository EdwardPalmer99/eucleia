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
class AddIntNode;
class AddFloatNode;
class AddBoolNode;
class AddStringNode;
class AddArrayNode;


class DataTypeSubParser : public SubParser
{
public:
    explicit DataTypeSubParser(FileParser &parser) : SubParser(parser) {}

    AddIntNode *parseInt();

    AddFloatNode *parseFloat();

    /* Parse: true/false */
    AddBoolNode *parseBool();

    /* Parse: "..." */
    AddStringNode *parseString();

    /* Parse: [1, 2, 3, 4] OR [true, false, true] OR [1.2, 2.4] OR ["hello, ", "world!"] */
    AddArrayNode *parseArray();
};