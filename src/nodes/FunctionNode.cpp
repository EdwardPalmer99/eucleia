/**
 * @file FunctionNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "FunctionNode.hpp"
#include "AnyObject.hpp"
#include "Logger.hpp"
#include "Scope.hpp"


/// Create a new FunctionObject from a FunctionNode and register in current scope.
AnyObject FunctionNode::evaluate(Scope &scope)
{
    log().info("evaluating reference in FunctionNode: " + _funcName);

    // TODO: - I think this creates a strong-reference cycle! Need to break the chain here
    auto functionObject = AnyObject(shared_from_this(), AnyObject::_UserFunction);

    log().debug("Linking function node with name " + _funcName);

    return scope.link(_funcName, std::move(functionObject)); // TODO: - this should be called in evaluateRef()
}
