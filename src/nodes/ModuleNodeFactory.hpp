//
//  ModuleNodeFactory.hpp
//  Eucleia
//
//  Created by Edward on 11/02/2024.
//

#pragma once
#include "AnyNode.hpp"
#include "Scope.hpp"
#include <string>

class AnyNode;

namespace NodeFactory
{

AnyNode *createDefinedModuleNode(std::string moduleName);

AnyNode *createIOModuleNode();

AnyNode *createMathModuleNode();

AnyNode *createArrayModuleNode();

AnyNode *createTestModuleNode();

} // namespace NodeFactory
