/**
 * @file StructNodeDefinition.hpp
 * @author Edward Palmer
 * @date 2024-11-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "AddVariableNode.hpp"
#include "BaseNode.hpp"
#include "BaseObject.hpp"
#include "ProgramNode.hpp"
#include "Scope.hpp"

/**
 * This node defines a new struct and its member variables. Similar in logic
 * to the FunctionNode. It adds the struct to the scope.
 */
class StructDefinitionNode : public BaseNode
{
public:
    StructDefinitionNode(BaseNode *structTypeName_, BaseNode *structMemberVars_)
        : structTypeName(static_cast<AddVariableNode *>(structTypeName_)),
          structMemberVars(static_cast<ProgramNode *>(structMemberVars_))
    {
    }

    ~StructDefinitionNode() override
    {
        delete structTypeName;
        delete structMemberVars;
    }

    // Creates a new StructObject and register in current scope.
    BaseObject *evaluate(Scope &scope) override;

    // TODO: - Inefficient. Better to extract what we need (i.e. string, etc).
    AddVariableNode *structTypeName{nullptr};

    /**
     * Basically a vector of AddVariableNode (one for each variable definition).
     * i.e. int a; float b; string c;
     */
    ProgramNode *structMemberVars{nullptr};
};