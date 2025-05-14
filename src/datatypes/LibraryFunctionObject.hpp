/**
 * @file ModuleFunctionObject.hpp
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
#include <string>

class Scope;

using ModuleFunction = std::function<BaseObject *(BaseNodePtrVector &, Scope &)>;
using ModuleFunctionPair = std::pair<std::string, ModuleFunction>;

/// Library function allows us to define lambdas which wrap around existing stdlib
/// functions. These can then be added to a global scope after seeing "import <...>"
/// with angled-brackets.
class ModuleFunctionObject : public BaseObjectT<ModuleFunction>
{
public:
    /* Set evaluate to be a reference to _value */
    ModuleFunctionObject(ModuleFunction function) : BaseObjectT<ModuleFunction>(function)
    {
    }

    ModuleFunctionObject *clone() const override
    {
        return new ModuleFunctionObject(_value);
    }

    /* Elegant operator overload allowing user to call function */
    BaseObject *operator()(BaseNodePtrVector &args, Scope &scope) const
    {
        return _value(args, scope);
    }
};