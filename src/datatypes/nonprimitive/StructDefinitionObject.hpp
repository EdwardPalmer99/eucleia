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
    StructDefinitionObject(std::vector<AddVariableNode *> variableDefs_)
        : variableDefs(std::move(variableDefs_)) {}

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
     * Store AddVariableNode so class can create all defined variables. We take
     * ownership of all nodes. To construct the object, we will call evaluate()
     * method on each node.
     */
    std::vector<AddVariableNode *> variableDefs;
};