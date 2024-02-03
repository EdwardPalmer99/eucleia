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


std::string Interpreter::evaluateTestFile(const std::string & fpath)
{
	// 1. Redirect std::out to oss.
	auto * bufferPtr = std::cout.rdbuf();

	std::ostringstream oss;
	std::cout.rdbuf(oss.rdbuf());
	
	// 2. Evaluate file.
	Interpreter::evaluateFile(fpath);
	
	// 3. Restore std::out buffer.
	std::cout.rdbuf(bufferPtr);

	// 4. Return string.
	return oss.str();
}


#pragma mark -
