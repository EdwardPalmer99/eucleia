/**
 * @file AddArrayNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "AddArrayNode.hpp"
#include "FileParser.hpp"
#include "ProgramNode.hpp"
#include <functional>

ArrayObject *AddArrayNode::evaluate(Scope &scope)
{
    std::vector<BaseObject *> evaluatedObjects;

    evaluatedObjects.reserve(programNodes.size());

    // Scope owns all objects in array we are passing in. The array object will
    // copy these!
    for (auto *node : programNodes)
    {
        evaluatedObjects.push_back(node->evaluate(scope));
    }

    return scope.createManagedObject<ArrayObject>(std::move(evaluatedObjects));
}


AddArrayNode *AddArrayNode::parse(FileParser &parser)
{
    auto programNodes = parser.parseDelimited("[", "]", ",", std::bind(&FileParser::parseExpression, parser));

    auto nodesVector = programNodes->releaseNodes();

    delete programNodes;

    return new AddArrayNode(nodesVector);
}
