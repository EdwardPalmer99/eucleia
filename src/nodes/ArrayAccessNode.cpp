/**
 * @file ArrayAccessNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "ArrayAccessNode.hpp"
#include "ArrayObject.hpp"
#include "BaseNode.hpp"
#include "BaseObjectT.hpp"
#include "FileParser.hpp"


ArrayAccessNode::~ArrayAccessNode()
{
    delete arrayLookup;
    delete index;
}


BaseObject *ArrayAccessNode::evaluateNoClone(Scope &scope)
{
    // Lookup in array.
    auto &arrayObj = arrayLookup->evaluate(scope)->castObject<ArrayObject>();

    return arrayObj[index->evaluateObject<IntObject::Type>(scope)];
}


BaseObject *ArrayAccessNode::evaluate(Scope &scope)
{
    BaseObject *currentObject = evaluateNoClone(scope);

    // Return a safe copy of object.
    return scope.cloneObject(currentObject);
}


ArrayAccessNode *ArrayAccessNode::parse(FileParser &parser, BaseNode *arrayVarName)
{
    auto &arrayName = arrayVarName->castNode<LookupVariableNode>();

    parser._skipFunctor("[");

    auto arrayIndex = static_cast<AddIntNode *>(parser.parseExpression());

    parser._skipFunctor("]");

    return new ArrayAccessNode(&arrayName, arrayIndex);
}
