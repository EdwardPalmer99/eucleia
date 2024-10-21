//
//  EucleiaInterpreter.cpp
//  Eucleia
//
//  Created by Edward on 25/01/2024.
//

#include "EucleiaParser.hpp"
#include "EucleiaObject.hpp"
#include "EucleiaScope.hpp"
#include "EucleiaInterpreter.hpp"
#include "EucleiaNode.hpp"
#include "EucleiaLibraries.hpp"
#include <iostream>
#include <sstream>

// TODO: - Parser() should have empty constructor. Should call parseFile method with string to run parser.
void Interpreter::evaluateFile(const std::string & fpath)
{
	// 1. Generate abstract symbol tree.
	auto ast = Parser::buildAbstractSymbolTree(fpath);
	
	// 2. Create global scope.
	Scope globalScope;
	
	// 3. Evaluate abstract symbol tree.
	std::ostringstream oss;
	
	auto * bufferPtr = std::cout.rdbuf();
	
	std::cout.rdbuf(oss.rdbuf());
	
	ast->evaluate(globalScope);
	
	std::cout.rdbuf(bufferPtr);
	
	std::cout << oss.str();
}


std::string Interpreter::evaluateString(const std::string fileContents)
{
	auto ast = Parser::buildAbstractSymbolTreeFromString(std::move(fileContents));

	Scope globalScope;

	// Redirect std::out to oss.
	auto * bufferPtr = std::cout.rdbuf();

	std::ostringstream oss;

	std::cout.rdbuf(oss.rdbuf());

	ast->evaluate(globalScope);

	// Restore std::out buffer.
	std::cout.rdbuf(bufferPtr);

	// Return string.
	return oss.str();
}


#pragma mark -
