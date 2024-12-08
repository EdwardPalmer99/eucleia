/**
 * @file FunctionNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "FunctionNode.hpp"
#include "FunctionObject.hpp"

/// Create a new FunctionObject from a FunctionNode and register in current scope.
BaseObject *FunctionNode::evaluate(Scope &scope)
{
    auto &funcNameNode = funcName->castNode<LookupVariableNode>();

    auto functionObject = scope.createManagedObject<FunctionObject>(this); // TODO: - uh-oh!! Think about pointer ownership here!!

    scope.linkObject(funcNameNode.name, functionObject);

    return functionObject;
}