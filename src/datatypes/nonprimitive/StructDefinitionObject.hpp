/**
 * @file StructDefinitionObject.hpp
 * @author Edward Palmer
 * @date 2024-11-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "AddVariableNode.hpp"
#include "BaseObject.hpp"
#include <unordered_map>
#include <vector>

/**
 * This class defines the format of a struct and the variables stored inside. It
 * will be stored in the scope along with the struct name. We can then use this
 * to construct struct instances.
 */
class StructDefinitionObject : public BaseObject
{
public:
    /**
     * Supply a vector of nodes for constructing the struct. This class will take
     * ownership and free these later.
     */
    StructDefinitionObject(std::vector<AddVariableNode *> variableDefs_,
                           StructDefinitionObject *parent_ = nullptr)
        : variableDefs(std::move(variableDefs_)),
          parent(parent_)
    {
        buildVariableDefHashMap();
    }

    /**
     * Destructor deletes all nodes in variable definitions.
     */
    ~StructDefinitionObject() override;

    /**
     * No destructor provided. Should not be possible to copy the struct definition
     * as you would expect. If this were to be implemented, all nodes we area
     * storing would have to be copied.
     */
    StructDefinitionObject *clone() const final
    {
        EucleiaError("not implemented.");
    }

    /**
     * Builds the variableDefsMap from any parent classes and checks for clashing
     * variables
     */
    void buildVariableDefHashMap();

    /**
     * Store AddVariableNode so class can create all defined variables. We take
     * ownership of all nodes. There may be additional nodes that are not in this
     * vector and will be stored in a parent class.
     */
    std::vector<AddVariableNode *> variableDefs;

    /**
     * Stores our owned variables and those of any parent variables we inherit.
     * To construct the object, we will call evaluate() method on each node.
     */
    std::unordered_map<std::string, AddVariableNode *> allVariableDefsMap;

    /**
     * Parent struct we inherit from.
     */
    StructDefinitionObject *parent{nullptr};
};