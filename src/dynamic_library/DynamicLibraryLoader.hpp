/**
 * @file DynamicLibraryLoader.hpp
 * @author Edward Palmer
 * @date 2025-04-08
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "BaseObject.hpp"
#include "ProgramNode.hpp"
#include "Scope.hpp"
#include <dlfcn.h>
#include <functional>
#include <string>
#include <vector>

/**
 * TODO: - an improvement idea: instead of having to add all functions to the scope, would be better to instead call
 * something like [module].func and then we'd realize that it's a function call.
 */


class DynamicLibraryLoader : public BaseNode
{
public:
    using FuncName = std::string;
    using Function = std::function<BaseObject *(ProgramNode &callArgs, Scope &scope)>;


    enum FuncSignature
    {
        DoubleFuncDouble,
        DoubleFuncDoubleDouble,
        DoubleFuncInt,
        IntFuncInt,
        IntFuncIntInt
    };

    using FuncDefinition = std::pair<FuncName, FuncSignature>;

    DynamicLibraryLoader() = delete;

    DynamicLibraryLoader(std::string libPath, std::initializer_list<FuncDefinition> funcDefinitions);
    ~DynamicLibraryLoader() override;

    /* Add all functions to the scope this is evaluated in */
    BaseObject *evaluate(Scope &scope) final;

protected:
    std::vector<std::pair<FuncName, Function>> _closures;

    void *_handle{nullptr};
};
