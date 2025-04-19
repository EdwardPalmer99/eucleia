/**
 * @file AddStringNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "AddStringNode.hpp"
#include "FileParser.hpp"

StringObject *AddStringNode::evaluate(Scope &scope)
{
    return scope.createManagedObject<StringObject>(stringObject);
}

AddStringNode *AddStringNode::parse(FileParser &parser)
{
    return new AddStringNode(parser.tokens().dequeue());
}
