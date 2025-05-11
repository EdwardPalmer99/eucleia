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
#include "AnyNode.hpp"
#include "ArrayAccessNode.hpp"
#include "FileParser.hpp"
#include "NodeFactory.hpp"
#include "Token.hpp"


AnyNode *DataTypeSubParser::parseInt()
{
    Token token = tokens().dequeue();

    long intValue = strtold(token.c_str(), NULL);

    return NodeFactory::createIntNode(intValue);
}


AnyNode *DataTypeSubParser::parseFloat()
{
    Token token = tokens().dequeue();

    double floatValue = strtof(token.c_str(), NULL);

    return NodeFactory::createFloatNode(floatValue);
}


AnyNode *DataTypeSubParser::parseBool()
{
    Token token = tokens().dequeue();

    bool state = (token == "true");

    return NodeFactory::createBoolNode(state);
}


AnyNode *DataTypeSubParser::parseString()
{
    Token token = tokens().dequeue();

    return NodeFactory::createStringNode(std::move(token));
}


AddArrayNode *DataTypeSubParser::parseArray()
{
    auto nodes = subparsers().block.parseDelimited("[", "]", ",", std::bind(&FileParser::parseExpression, &parent()));

    return new AddArrayNode(nodes);
}


ArrayAccessNode *DataTypeSubParser::parseArrayAccessor(BaseNode *lastExpression)
{
    auto arrayName = static_cast<LookupVariableNode *>(lastExpression);

    skip("[");

    auto arrayIndex = parent().parseExpression();

    skip("]");

    return new ArrayAccessNode(arrayName, arrayIndex);
}
