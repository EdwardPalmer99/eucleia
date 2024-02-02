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

// TODO: - Parser() should have empty constructor. Should call parseFile method with string to run parser.
void Interpreter::evaluateFile(const std::string & fpath)
{
	// 1. Generate abstract symbol tree.
	auto ast = Parser::buildAbstractSymbolTree(fpath);
	
	// 2. Create global scope.
	Scope globalScope;
	
	// 3. Evaluate abstract symbol tree.
	ast->evaluate(globalScope);
}

// TODO: - add versions for evaluating strings.


#pragma mark -
