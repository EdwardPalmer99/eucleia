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
#include "NodeFactory.hpp"
#include "Token.hpp"


AnyNode DataTypeSubParser::parse(int type, AnyNodeOptional lastExpr)
{
    switch (type)
    {
        case Bool:
            return parseBool();
        case Int:
            return parseInt();
        case Float:
            return parseFloat();
        case String:
            return parseString();
        case Array:
            return parseArray();
        case ArrayAccessor:
            ThrowException("not handled yet");
        default:
            ThrowException("invalid parse type");
    }
}


AnyNode DataTypeSubParser::parseInt()
{
    Token token = tokens().dequeue();

    long intValue = strtold(token.c_str(), NULL);

    return NodeFactory::Instances::createIntNode(intValue);
}


AnyNode DataTypeSubParser::parseFloat()
{
    Token token = tokens().dequeue();

    double floatValue = strtof(token.c_str(), NULL);

    return NodeFactory::Instances::createFloatNode(floatValue);
}


AnyNode DataTypeSubParser::parseBool()
{
    Token token = tokens().dequeue();

    bool state = (token == "true");

    return NodeFactory::Instances::createBoolNode(state);
}


AnyNode DataTypeSubParser::parseString()
{
    Token token = tokens().dequeue();

    return NodeFactory::Instances::createStringNode(token);
}


AnyNode DataTypeSubParser::parseArray()
{
    auto programNodes = parseDelimited("[", "]", ",", std::bind(&FileParser::parseExpression, &parent()));

    return NodeFactory::Instances::createArrayNode(std::move(programNodes));
}


ArrayAccessNode *DataTypeSubParser::parseArrayAccessor(BaseNode *lastExpression)
{
    auto arrayName = static_cast<LookupVariableNode *>(lastExpression);

    skip("[");

    auto arrayIndex = static_cast<AddIntNode *>(parent().parseExpression());

    skip("]");

    return new ArrayAccessNode(arrayName, arrayIndex);
}
