//
//  EucleiaInterpreter.cpp
//  Eucleia
//
//  Created by Edward on 25/01/2024.
//

#include "EucleiaInterpreter.hpp"
#include "AnyNode.hpp"
#include "FileParser.hpp"
#include "Scope.hpp"

namespace Interpreter
{

void evaluateFile(const std::string &fpath)
{
    // 1. Generate abstract symbol tree.
    AnyNode ast = FileParser::parseMainFile(fpath);

    // 2. Create global scope.
    Scope globalScope;

    // 3. Evaluate AST.
    (void)ast(globalScope);
}

} // namespace Interpreter