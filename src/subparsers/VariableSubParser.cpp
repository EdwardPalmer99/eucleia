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
#include "FileParser.hpp"
#include "LookupVariableNode.hpp"
#include "Token.hpp"

BaseNode *VariableSubParser::parseVariableDefinition()
{
    Token typeToken = tokens().dequeue();
    assert(typeToken.type() == Token::Keyword);

    ObjectType typeOfObject = objectTypeForName(typeToken);

    if (tokens().front() == "&") // Is reference.
    {
        return parseReference(typeOfObject);
    }

    Token nameToken = tokens().dequeue();
    assert(nameToken.type() == Token::Variable);

    return new AddVariableNode(nameToken, typeOfObject);
}


BaseNode *VariableSubParser::parseReference(ObjectType boundVariableType)
{
    skip("&");

    Token referenceNameToken = tokens().dequeue();
    assert(referenceNameToken.type() == Token::Variable);

    skip("=");

    Token boundVariableNameToken = tokens().dequeue();
    assert(boundVariableNameToken.type() == Token::Variable);

    return new AddReferenceVariableNode(referenceNameToken, boundVariableNameToken, boundVariableType);
}


BaseNode *VariableSubParser::parseVariable()
{
    Token token = tokens().dequeue();
    assert(token.type() == Token::Variable);

    return new LookupVariableNode(token);
}