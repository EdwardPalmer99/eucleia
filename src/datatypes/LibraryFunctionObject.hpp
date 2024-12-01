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
#include <functional>

class ProgramNode;
class Scope;

/// Library function allows us to define lambdas which wrap around existing stdlib
/// functions. These can then be added to a global scope after seeing "import <...>"
/// with angled-brackets.
class LibraryFunctionObject : public BaseObject
{
public:
    using LibraryFunction = std::function<BaseObject *(ProgramNode &, Scope &)>;

    LibraryFunctionObject(LibraryFunction function_) : evaluate(std::move(function_)) {}

    LibraryFunctionObject *clone() const override
    {
        return new LibraryFunctionObject(evaluate);
    }

    LibraryFunction evaluate;
};