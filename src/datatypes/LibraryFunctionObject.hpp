/**
 * @file LibraryFunctionObject.hpp
 * @author Edward Palmer
 * @date 2024-11-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseObject.hpp"
#include "BaseObjectT.hpp"
#include <functional>

class Scope;

using LibraryFunction = std::function<BaseObject *(BaseNodePtrVector &, Scope &)>;

/// Library function allows us to define lambdas which wrap around existing stdlib
/// functions. These can then be added to a global scope after seeing "import <...>"
/// with angled-brackets.
class LibraryFunctionObject : public BaseObjectT<LibraryFunction>
{
public:
    /* Set evaluate to be a reference to _value */
    LibraryFunctionObject(LibraryFunction function) : BaseObjectT<LibraryFunction>(function)
    {
    }

    LibraryFunctionObject *clone() const override
    {
        return new LibraryFunctionObject(_value);
    }

    /* Elegant operator overload allowing user to call function */
    BaseObject *operator()(BaseNodePtrVector &args, Scope &scope) const
    {
        return _value(args, scope);
    }
};