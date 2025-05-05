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


class DataTypeSubParser : public SubParser
{
public:
    explicit DataTypeSubParser(FileParser &parser) : SubParser(parser) {}

    AddIntNode *parseInt();

    AddFloatNode *parseFloat();

    AddBoolNode *parseBool();

    AddStringNode *parseString();
};