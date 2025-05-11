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
#include "Logger.hpp"
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


AnyNode *DataTypeSubParser::parseArray()
{
    log().debug("parsing array...");
    BaseNodePtrVector nodes = subparsers().block.parseDelimited("[", "]", ",", std::bind(&FileParser::parseExpression, &parent()));
    log().debug("nodes.size() = " + std::to_string(nodes.size()));
    return NodeFactory::createArrayNode(toSharedNodePtrVector(nodes));
}


ArrayAccessNode *DataTypeSubParser::parseArrayAccessor(BaseNode *lastExpression)
{
    auto arrayName = static_cast<LookupVariableNode *>(lastExpression);

    skip("[");

    auto arrayIndex = parent().parseExpression();

    skip("]");

    return new ArrayAccessNode(arrayName, arrayIndex);
}
