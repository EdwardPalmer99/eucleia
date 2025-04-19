/**
 * @file FunctionNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "FunctionNode.hpp"
#include "FileParser.hpp"
#include "FunctionObject.hpp"
#include "ProgramNode.hpp"

/// Create a new FunctionObject from a FunctionNode and register in current scope.
BaseObject *FunctionNode::evaluate(Scope &scope)
{
    auto &funcNameNode = funcName->castNode<LookupVariableNode>();

    auto functionObject = scope.createManagedObject<FunctionObject>(this); // TODO: - uh-oh!! Think about pointer ownership here!!

    scope.linkObject(funcNameNode.name, functionObject);

    return functionObject;
}


FunctionNode *FunctionNode::parse(FileParser &parser)
{
    parser._skipFunctor("func");

    auto funcName = new LookupVariableNode(parser.tokens().dequeue());
    auto funcArgs = parser.parseDelimited("(", ")", ",", std::bind(&FileParser::parseVariableDefinition, &parser)); // Func variables.
    auto funcBody = ProgramNode::parse(parser, true);

    return new FunctionNode(funcName, funcArgs, funcBody);
}
