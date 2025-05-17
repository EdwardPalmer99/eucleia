/**
 * @file DataTypeSubParser.cpp
 * @author Edward Palmer
 * @date 2025-04-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "DataTypeSubParser.hpp"
#include "AnyNode.hpp"
#include "FileParser.hpp"
#include "Logger.hpp"
#include "NodeFactory.hpp"
#include "Token.hpp"


AnyNode::Ptr DataTypeSubParser::parseInt()
{
    Token token = tokens().dequeue();

    long intValue = strtold(token.c_str(), NULL);

    return NodeFactory::createIntNode(intValue);
}


AnyNode::Ptr DataTypeSubParser::parseFloat()
{
    Token token = tokens().dequeue();

    double floatValue = strtof(token.c_str(), NULL);

    return NodeFactory::createFloatNode(floatValue);
}


AnyNode::Ptr DataTypeSubParser::parseBool()
{
    Token token = tokens().dequeue();

    bool state = (token == "true");

    return NodeFactory::createBoolNode(state);
}


AnyNode::Ptr DataTypeSubParser::parseString()
{
    Token token = tokens().dequeue();

    return NodeFactory::createStringNode(std::move(token));
}


AnyNode::Ptr DataTypeSubParser::parseArray()
{
    BaseNodePtrVector nodes = subparsers().block.parseDelimited("[", "]", ",", std::bind(&FileParser::parseExpression, &parent()));
    return NodeFactory::createArrayNode(std::move(nodes));
}


AnyPropertyNode::Ptr DataTypeSubParser::parseArrayAccessor(BaseNode::Ptr lastExpression)
{
    skip("[");

    auto arrayIndexNode = parent().parseExpression();

    skip("]");

    return NodeFactory::createArrayAccessNode(lastExpression, arrayIndexNode);
}
