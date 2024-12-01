/**
 * @file ClassDefinitionNode.hpp
 * @author Edward Palmer
 * @date 2024-11-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "AddVariableNode.hpp"
#include "BaseNode.hpp"
#include "BaseObject.hpp"
#include "FunctionNode.hpp"
#include "Scope.hpp"
#include "StructDefinitionNode.hpp"
#include <string>
#include <vector>

/**
 * This node defines a new struct and its member variables. Similar in logic to
 * the StructDefinitionNode.
 */
class ClassDefinitionNode : public StructDefinitionNode
{
public:
    /**
     * Pass in the name for the class type and a vector of variables and methods.
     * NB: we take ownership of these nodes!
     */
    ClassDefinitionNode(std::string typeName_,
                        std::vector<AddVariableNode *> classVariables_,
                        std::vector<FunctionNode *> classMethods_);

    ~ClassDefinitionNode();

    /**
     *  Creates a new ClassDefinitionObject and register in current scope.
     */
    BaseObject *evaluate(Scope &scope) override;

    std::vector<FunctionNode *> methodDefs;
};
