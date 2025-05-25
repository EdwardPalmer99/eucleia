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

ModuleFunctor::ModuleFunctor(Function function) : _function(function) {}

std::shared_ptr<AnyObject> ModuleFunctor::operator()(BaseNodePtrVector &args, Scope &scope)
{
    return !_function ? nullptr : _function(args, scope);
}