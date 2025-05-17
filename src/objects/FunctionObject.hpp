/**
 * @file FunctionObject.hpp
 * @author Edward Palmer
 * @date 2024-11-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseObjectT.hpp"
#include "ObjectFactory.hpp"

/* Forward declaration */
class FunctionNode;

using FunctionNodePtr = FunctionNode *;

/// FunctionObject contains a pointer to the original function definition which
/// allows us to call its evaluate() method and perform type-checking of the
/// supplied function arguments with the expected arguments.
class FunctionObject : public BaseObjectT<FunctionNodePtr>
{
public:
    FunctionObject(FunctionNodePtr function) : BaseObjectT<FunctionNodePtr>(function) {}

    FunctionObject::Ptr clone() const override
    {
        return ObjectFactory::allocate<FunctionObject>(_value);
    }
};