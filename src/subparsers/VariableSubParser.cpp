/**
 * @file VariableSubParser.cpp
 * @author Edward Palmer
 * @date 2025-05-05
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "VariableSubParser.hpp"
#include "AddVariableNode.hpp"
#include "AnyNode.hpp"
#include "ClassNode.hpp"
#include "FileParser.hpp"
#include "LookupVariableNode.hpp"
#include "NodeFactory.hpp"
#include "Token.hpp"
#include <cassert>


BaseNode::Ptr VariableSubParser::parseVariableDefinition()
{
    Token typeToken = tokens().dequeue();
    assert(typeToken.type() == Token::Keyword);

    AnyObject::Type typeOfObject = AnyObject::stringToType(typeToken);

    if (tokens().front() == "&") // Is reference: [type] & [name] = [another object]
    {
        return parseReference(typeOfObject);
    }
    else if (tokens().front() == "(") // Is cast: i.e., float(1), float(intVar), bool(1), int(1.2)
    {
        return parseCast(typeOfObject);
    }

    Token nameToken = tokens().dequeue();
    assert(nameToken.type() == Token::Variable);

    return std::make_shared<AddVariableNode>(nameToken, typeOfObject);
}


#include <iostream>


BaseNode::Ptr VariableSubParser::parseReference(AnyObject::Type boundVariableType)
{
    skip("&"); // TODO: - add type-checking

    Token referenceNameToken = tokens().dequeue();
    assert(referenceNameToken.type() == Token::Variable);

    skip("=");

    Token boundVariableNameToken = tokens().dequeue();
    assert(boundVariableNameToken.type() == Token::Variable);

    return std::make_shared<AddReferenceVariableNode>(referenceNameToken, boundVariableNameToken, boundVariableType);
}


BaseNode::Ptr VariableSubParser::parseVariable()
{
    Token token = tokens().dequeue();
    assert(token.type() == Token::Variable);

    /* Note: we may have a class instance defined here: [classType] [classInstance] */
    if (subparsers().classParser.isParsedClassDefinition(token))
    {
        /* ClassTypeName, ClassInstanceName */
        return std::make_shared<ClassNode>(token, tokens().dequeue());
    }

    return std::make_shared<LookupVariableNode>(token);
}


BaseNode::Ptr VariableSubParser::parseCast(AnyObject::Type castType)
{
    /* Expression in ( expression ) to cast return type of */
    auto expression = parent().parseBrackets();

    return NodeFactory::createCastNode(expression, castType);
}
