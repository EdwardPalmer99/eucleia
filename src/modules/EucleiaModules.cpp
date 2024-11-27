//
//  EucleiaModuleLoader.cpp
//  Eucleia
//
//  Created by Edward on 11/02/2024.
//

#include "EucleiaModules.hpp"
#include "ProgramNode.hpp"
#include "TestModule.hpp"
#include <cmath>
#include <iostream>
#include <memory>

std::unique_ptr<EucleiaModuleLoader> EucleiaModuleLoader::_instance = nullptr;


BaseObject *ModuleNode::evaluate(Scope &scope)
{
    defineFunctions();

    // Iterate over functions and add to scope.
    for (const auto &[name, function] : _functionsMap)
    {
        LibraryFunctionObject *object = scope.createManagedObject<LibraryFunctionObject>(function);
        scope.linkObject(name, object);
    }

    return nullptr;
}


void ModuleNode::defineFunction(const std::string &name, Function function)
{
    _functionsMap[name] = std::move(function);
}


std::vector<BaseObject *> ModuleNode::evaluateArgs(ProgramNode &args, Scope &scope) const
{
    std::vector<BaseObject *> out(args.programNodes.size());

    int i = 0;
    for (const auto &node : args.programNodes)
    {
        out[i++] = node->evaluate(scope);
    }

    return out;
}


void MathModuleNode::defineFunctions()
{
    defineFunction("sqrt", [*this](ProgramNode &callArgs, Scope &scope) -> BaseObject *
                   {
    	auto argValues = evaluateArgs(callArgs, scope);
    	assert(argValues.size() == 1);

    	auto & first = argValues.front()->castObject<FloatObject>();
    	return scope.createManagedObject<FloatObject>(sqrt(first.value)); });

    defineFunction("pow", [*this](ProgramNode &callArgs, Scope &scope) -> BaseObject *
                   {
    	auto argValues = evaluateArgs(callArgs, scope);
    	assert(argValues.size() == 2);

    	auto &first = argValues.at(0)->castObject<FloatObject>();
    	auto &second = argValues.at(1)->castObject<FloatObject>();

    	return scope.createManagedObject<FloatObject>(pow(first.value, second.value)); });
}


void IOModuleNode::defineFunctions()
{
    auto closure = [](ProgramNode &callArgs, Scope &scope) -> BaseObject *
    {
        for (const auto &node : callArgs.programNodes)
        {
            auto evaluatedNode = node->evaluate(scope);

            std::cout << *evaluatedNode;

            if (node != callArgs.programNodes.back())
            {
                std::cout << " ";
            }
        }

        std::cout << std::endl;
        return nullptr;
    };

    defineFunction("print", closure);
}


ModuleNode *EucleiaModuleLoader::getModule(const std::string &name) const
{
    if (name == "io")
        return new IOModuleNode();
    else if (name == "math")
        return new MathModuleNode();
    else if (name == "test")
        return new TestModule();
    else
        EucleiaError("No library with name '%s'.\n", name.c_str());
}
