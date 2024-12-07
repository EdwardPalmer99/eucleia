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
#include "BaseNode.hpp"
#include "BaseObject.hpp"
#include <unordered_map>
#include <unordered_set>
#include <vector>

/**
 * This class defines the format of a struct and the variables stored inside. It
 * will be stored in the scope along with the struct name. We can then use this
 * to construct struct instances.
 */
class StructDefinitionObject : public BaseObject, public BaseNode
{
public:
    /**
     * Supply a vector of nodes for constructing the struct. This class will take
     * ownership and free these later.
     */
    StructDefinitionObject(std::string typeName_,
                           std::string parentTypeName_,
                           std::vector<AddVariableNode *> variableDefs_);

    /**
     * Destructor deletes all nodes in variable definitions.
     */
    ~StructDefinitionObject() override;

    /**
     * Registers this class in the current scope with name. Ownership will pass
     * to the scope. Careful! Going this route means we don't have to create a
     * separate Node to create an Object.
     */
    BaseObject *evaluate(Scope &scope) override;

    /**
     * No destructor provided. Should not be possible to copy the struct definition
     * as you would expect. If this were to be implemented, all nodes we are
     * storing would have to be copied.
     */
    StructDefinitionObject *clone() const final
    {
        EucleiaError("not implemented.");
    }

    /**
     * Calls evaluate method on all variables in this struct and parents. Installs
     * them in argument scope.
     */
    void installVariablesInScope(Scope &scope, std::unordered_set<std::string> &variableNames) const;

protected:
    /**
     * Builds the variableDefsMap from any parent classes and checks for clashing
     * variables.
     */
    void buildVariableDefHashMap(const Scope &scope);

    /**
     * Returns a pointer to the parent struct or nullptr if not found.
     */
    StructDefinitionObject *lookupParent(const Scope &scope) const;

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
     * Type name for struct.
     */
    std::string typeName;

    /**
     * Type name for parent.
     */
    std::string parentTypeName;

    /**
     * We activate the definition once evaluate is called. This is when we can
     * try to locate the definition of the parent if there is one and install
     * it in the scope. We also refuse to allow more than one definition in the
     * scope as you would expect so evaluate() cannot be called more than once!
     */
    bool active{false};
};