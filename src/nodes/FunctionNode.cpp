/**
 * @file FunctionNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "FunctionNode.hpp"

/// Create a new FunctionObject from a FunctionNode and register in current scope.
BaseObject *FunctionNode::evaluate(Scope &scope)
{
    auto &funcNameNode = funcName->castNode<AddNewVariableNode>();

    auto functionObject = scope.createManagedObject<FunctionObject>(this); // TODO: - uh-oh!! Think about pointer ownership here!!

    scope.defineObject(funcNameNode.variableName, functionObject);

    return functionObject;
}