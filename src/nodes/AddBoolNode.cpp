/**
 * @file AddBoolNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "AddBoolNode.hpp"
#include "FileParser.hpp"

BoolObject *AddBoolNode::evaluate(Scope &scope)
{
    return scope.createManagedObject<BoolObject>(boolObject);
}


AddBoolNode *AddBoolNode::parse(FileParser &parser)
{
    Token token = parser.tokens().dequeue();
    return new AddBoolNode(token.toBool());
}
