/**
 * @file DataTypeSubParser.cpp
 * @author Edward Palmer
 * @date 2025-04-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "DataTypeSubParser.hpp"
#include "AddArrayNode.hpp"
#include "AddBoolNode.hpp"
#include "AddFloatNode.hpp"
#include "AddIntNode.hpp"
#include "AddStringNode.hpp"
#include "ArrayAccessNode.hpp"
#include "FileParser.hpp"
#include "Token.hpp"


AddIntNode *DataTypeSubParser::parseInt()
{
    Token token = tokens().dequeue();

    long intValue = strtold(token.c_str(), NULL);

    return new AddIntNode(intValue);
}


AddFloatNode *DataTypeSubParser::parseFloat()
{
    Token token = tokens().dequeue();

    double floatValue = strtof(token.c_str(), NULL);

    return new AddFloatNode(floatValue);
}


AddBoolNode *DataTypeSubParser::parseBool()
{
    Token token = tokens().dequeue();

    bool state = (token == "true");

    return new AddBoolNode(state);
}


AddStringNode *DataTypeSubParser::parseString()
{
    Token token = tokens().dequeue();

    return new AddStringNode(token);
}


AddArrayNode *DataTypeSubParser::parseArray()
{
    auto programNodes = parseDelimited("[", "]", ",", std::bind(&FileParser::parseExpression, &parent()));

    auto nodesVector = programNodes->releaseNodes();

    delete programNodes;

    return new AddArrayNode(nodesVector);
}


ArrayAccessNode *DataTypeSubParser::parseArrayAccessor(BaseNode *lastExpression)
{
    auto arrayName = static_cast<LookupVariableNode *>(lastExpression);

    skip("[");

    auto arrayIndex = static_cast<AddIntNode *>(parent().parseExpression());

    skip("]");

    return new ArrayAccessNode(arrayName, arrayIndex);
}
