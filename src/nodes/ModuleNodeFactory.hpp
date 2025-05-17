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

namespace NodeFactory
{

AnyNode::Ptr createDefinedModuleNode(std::string moduleName);

AnyNode::Ptr createIOModuleNode();

AnyNode::Ptr createMathModuleNode();

AnyNode::Ptr createArrayModuleNode();

AnyNode::Ptr createTestModuleNode();

} // namespace NodeFactory
