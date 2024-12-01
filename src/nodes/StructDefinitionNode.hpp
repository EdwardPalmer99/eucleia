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
#include "Scope.hpp"
#include <string>
#include <vector>

// TODO: - merge this with StructDefinitionObject.

/**
 * This node defines a new struct and its member variables. Similar in logic
 * to the FunctionNode. It adds the struct to the scope.
 */
class StructDefinitionNode : public BaseNode
{
public:
    StructDefinitionNode(std::string typeName_,
                         std::vector<AddVariableNode *> variableDefs_)
        : typeName(std::move(typeName_)),
          variableDefs(std::move(variableDefs_))
    {
    }

    /**
     * Deletes nodes stored in variableDefs IFF evaluate not called.
     */
    ~StructDefinitionNode() override;

    /*
     * Creates a new StructDefinitionObject and register in current scope.
     */
    BaseObject *evaluate(Scope &scope) override;

    /**
     * Type name for struct.
     */
    std::string typeName{nullptr};

    /**
     * Stores nodes defined in struct.
     */
    std::vector<AddVariableNode *> variableDefs;

    /**
     * Prevent evaluate being called multiple times as vector ownership changes
     * once called.
     */
    bool evaluateCalled{false};
};