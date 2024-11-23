/**
 * @file FunctionObject.hpp
 * @author Edward Palmer
 * @date 2024-11-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseObject.hpp"

class FunctionNode; // NB: - not ideal. Move FunctionNode into separate file.

/// FunctionObject contains a pointer to the original function definition which
/// allows us to call its evaluate() method and perform type-checking of the
/// supplied function arguments with the expected arguments.
class FunctionObject : public BaseObject
{
public:
    FunctionObject(FunctionNode *function_) : functionValue(function_) {}

    FunctionObject *clone() const override
    {
        return new FunctionObject(functionValue);
    }

    FunctionNode *functionValue{nullptr}; // TODO: - weird code.
};