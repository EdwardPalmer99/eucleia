/**
 * @file DataTypeSubParser.cpp
 * @author Edward Palmer
 * @date 2025-04-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "DataTypeSubParser.hpp"
#include "AddBoolNode.hpp"
#include "AddFloatNode.hpp"
#include "AddIntNode.hpp"
#include "AddStringNode.hpp"
#include "FileParser.hpp"
#include "Token.hpp"


AddIntNode *DataTypeSubParser::parseInt()
{
    Token token = _parser._tokens.dequeue();

    long intValue = strtold(token.c_str(), NULL);

    return new AddIntNode(intValue);
}


AddFloatNode *DataTypeSubParser::parseFloat()
{
    Token token = _parser._tokens.dequeue();

    double floatValue = strtof(token.c_str(), NULL);

    return new AddFloatNode(floatValue);
}


AddBoolNode *DataTypeSubParser::parseBool()
{
    Token token = _parser._tokens.dequeue();

    bool state = (token == "true");

    return new AddBoolNode(state);
}


AddStringNode *DataTypeSubParser::parseString()
{
    Token token = _parser._tokens.dequeue();

    return new AddStringNode(token);
}