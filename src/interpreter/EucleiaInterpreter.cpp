//
//  EucleiaInterpreter.cpp
//  Eucleia
//
//  Created by Edward on 25/01/2024.
//

#include "EucleiaInterpreter.hpp"
#include "BaseObject.hpp"
#include "EucleiaModules.hpp"
#include "FileParser.hpp"
#include "Objects.hpp"
#include "Scope.hpp"
#include "TopParser.hpp"
#include <iostream>

// TODO: - Parser() should have empty constructor. Should call parseFile method with string to run parser.
void Interpreter::evaluateFile(const std::string &fpath)
{
    // 1. Generate abstract symbol tree.
    FileNode *ast = RootParser::instance().parse(fpath);

    // 2. Create global scope.
    Scope globalScope;

    // 3. Evaluate AST.
    (void)ast->evaluate(globalScope);

    // 4. Memory cleanup.
    delete ast;
}
