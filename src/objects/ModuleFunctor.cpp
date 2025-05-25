/**
 * @file ModuleFunctor.cpp
 * @author Edward Palmer
 * @date 2025-05-19
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "ModuleFunctor.hpp"
#include "AnyObject.hpp"
#include "BaseNode.hpp"

ModuleFunctor::ModuleFunctor(Function function) : _function(function) {}

AnyObject ModuleFunctor::operator()(BaseNodePtrVector &args, Scope &scope)
{
    return !_function ? AnyObject() : _function(args, scope);
}