//
//  EucleiaModuleLoader.cpp
//  Eucleia
//
//  Created by Edward on 11/02/2024.
//

#include "EucleiaModules.hpp"
#include "TestModule.hpp"
#include <iostream>
#include <cmath>
#include <memory>

std::unique_ptr<EucleiaModuleLoader> EucleiaModuleLoader::_instance = nullptr;


std::shared_ptr<BaseObject> ModuleNode::evaluate(Scope & scope)
{
	defineFunctions();
	
	// Iterate over functions and add to scope.
	for (const auto & [name, function] : _functionsMap)
	{
		scope.defineObject(name, std::make_shared<LibraryFunctionObject>(function));
	}
	
	return nullptr;
}


void ModuleNode::defineFunction(const std::string & name, Function function)
{
	_functionsMap[name] = std::move(function);
}


std::vector<std::shared_ptr<BaseObject>> ModuleNode::evaluateArgs(ProgramNode & args, Scope & scope) const
{
	std::vector<std::shared_ptr<BaseObject>> out(args.nodes.size());
	
	int i = 0;
	for (const auto & node : args.nodes)
	{
		out[i++] = node->evaluate(scope);
	}
	
	return out;
}


void MathModuleNode::defineFunctions()
{
	defineFunction("sqrt", [=](ProgramNode & callArgs, Scope & scope) -> std::shared_ptr<BaseObject>
	{
		auto argValues = evaluateArgs(callArgs, scope);
		assert(argValues.size() == 1);
		
		auto first = argValues.front()->floatCast();
		return std::make_shared<FloatObject>(sqrt(first));
	});
	
	defineFunction("pow", [=](ProgramNode & callArgs, Scope & scope) -> std::shared_ptr<BaseObject>
	{
		auto argValues = evaluateArgs(callArgs, scope);
		assert(argValues.size() == 2);
		
		auto first = argValues.at(0)->floatCast();
		auto second = argValues.at(1)->floatCast();
		
		return std::make_shared<FloatObject>(pow(first, second));
	});
}


void IOModuleNode::defineFunctions()
{
	auto closure = [](ProgramNode & callArgs, Scope & scope) -> std::shared_ptr<BaseObject>
	{
		for (const auto & node : callArgs.nodes)
		{
			auto evaluatedNode = node->evaluate(scope);
			
			std::cout << *evaluatedNode;
			
			if (node != callArgs.nodes.back())
			{
				std::cout << " ";
			}
		}
		
		std::cout << std::endl;
		return nullptr;
	};
	
	defineFunction("print", closure);
}


std::shared_ptr<ModuleNode> EucleiaModuleLoader::getModule(const std::string & name) const
{
	auto it = _modules.find(name);
	if (it == _modules.end())
	{
		printWarpError("No library with name '%s'.\n", name.c_str());
	}
	
	return it->second;
}


void EucleiaModuleLoader::buildDefaultLibraries()
{
	_modules["io"] = std::make_shared<IOModuleNode>();
	_modules["math"] = std::make_shared<MathModuleNode>();
	_modules["test"] = std::make_shared<TestModule>();
	// Add test library.
}
