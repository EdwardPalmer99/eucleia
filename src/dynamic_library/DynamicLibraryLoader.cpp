/**
 * @file DynamicLibraryLoader.cpp
 * @author Edward Palmer
 * @date 2025-04-08
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "DynamicLibraryLoader.hpp"
#include "Exceptions.hpp"
#include "FloatObject.hpp"
#include "IntObject.hpp"
#include "LibraryFunctionObject.hpp"
#include "Logger.hpp"
#include "Stringify.hpp"

/* TODO: - add GTests and move to Bazel to test operation */

DynamicLibraryLoader::DynamicLibraryLoader(std::string libPath, std::initializer_list<FuncDefinition> funcDefinitions)
{
    /* Useful typedefs */
    typedef double (*DoubleFuncDoublePtr)(double);
    typedef double (*DoubleFuncDoubleDoublePtr)(double, double);
    typedef double (*DoubleFuncIntPtr)(int);
    typedef int (*IntFuncIntPtr)(int);
    typedef int (*IntFuncIntIntPtr)(int, int);

    _closures.clear();
    _closures.reserve(funcDefinitions.size());

    _handle = dlopen(libPath.c_str(), RTLD_LAZY);
    if (!_handle)
    {
        ThrowException("failed to load shared library with path: " + libPath + " with error: " + eucleia::stringify(dlerror()));
    }

    for (auto &[funcName, funcSignature] : funcDefinitions)
    {
        void *ptr = dlsym(_handle, funcName.c_str());
        if (!ptr)
        {
            ThrowException("failed to load function with name " + funcName);
        }

        /* Create each closure */
        auto closure = [ptr, funcSignature](ProgramNode &callArgs, Scope &scope) -> BaseObject *
        {
            switch (funcSignature)
            {
                case DoubleFuncDouble:
                {
                    auto func = reinterpret_cast<DoubleFuncDoublePtr>(ptr);

                    double returnValue = func(callArgs[0]->evaluate(scope)->castObject<FloatObject>().value);

                    return scope.createManagedObject<FloatObject>(returnValue);
                }
                case DoubleFuncDoubleDouble:
                {
                    auto func = reinterpret_cast<DoubleFuncDoubleDoublePtr>(ptr);

                    double returnValue = func(callArgs[0]->evaluate(scope)->castObject<FloatObject>().value,
                                              callArgs[1]->evaluate(scope)->castObject<FloatObject>().value);

                    return scope.createManagedObject<FloatObject>(returnValue);
                }
                case DoubleFuncInt:
                {
                    auto func = reinterpret_cast<DoubleFuncIntPtr>(ptr);

                    double returnValue = func(callArgs[0]->evaluate(scope)->castObject<IntObject>().value);

                    return scope.createManagedObject<FloatObject>(returnValue);
                }
                case IntFuncInt:
                {
                    auto func = reinterpret_cast<IntFuncIntPtr>(ptr);

                    int returnValue = func(callArgs[0]->evaluate(scope)->castObject<IntObject>().value);

                    return scope.createManagedObject<IntObject>(returnValue);
                }
                case IntFuncIntInt:
                {
                    auto func = reinterpret_cast<IntFuncIntIntPtr>(ptr);

                    int returnValue = func(callArgs[0]->evaluate(scope)->castObject<IntObject>().value,
                                           callArgs[1]->evaluate(scope)->castObject<IntObject>().value);

                    return scope.createManagedObject<IntObject>(returnValue);
                }
                default:
                {
                    ThrowException("unsupported function signature");
                }
            }
        };

        /* Store closures for when evaluate() is called */
        _closures.emplace_back(funcName, closure);
    }
}


BaseObject *DynamicLibraryLoader::evaluate(Scope &scope)
{
    for (const auto &[name, closure] : _closures)
    {
        Logger::debug("adding dynamic function " + name + " to scope");
        LibraryFunctionObject *object = scope.createManagedObject<LibraryFunctionObject>(closure);
        scope.linkObject(name, object);
    }

    return nullptr;
}


DynamicLibraryLoader::~DynamicLibraryLoader()
{
    if (!_handle)
    {
        return;
    }

    dlclose(_handle);
}
