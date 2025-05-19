/**
 * @file StructDefinitionNode.hpp
 * @author Edward Palmer
 * @date 2024-11-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseNode.hpp"
#include "Exceptions.hpp"
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/**
 * This class defines the format of a struct and the variables stored inside. It
 * will be stored in the scope along with the struct name. We can then use this
 * to construct struct instances.
 */
class StructDefinitionNode : public BaseNode, public std::enable_shared_from_this<StructDefinitionNode>
{
public:
    using Ptr = std::shared_ptr<StructDefinitionNode>;

    /**
     * Supply a vector of nodes for constructing the struct. This class will take
     * ownership and free these later.
     */
    StructDefinitionNode(std::string typeName_,
                         std::string parentTypeName_,
                         std::vector<std::shared_ptr<class AddVariableNode>> variableDefs_);

    /**
     * Destructor deletes all nodes in variable definitions.
     */
    ~StructDefinitionNode() override = default;

    /**
     * Registers this class in the current scope with name. Ownership will pass
     * to the scope. Careful! Going this route means we don't have to create a
     * separate Node to create an Object.
     */
    std::shared_ptr<class AnyObject> evaluate(class Scope &scope) override;

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
    std::shared_ptr<StructDefinitionNode> lookupParent(const Scope &scope) const;

    /**
     * Stores our owned variables and those of any parent variables we inherit.
     * To construct the object, we will call evaluate() method on each node.
     */
    std::unordered_map<std::string, std::shared_ptr<class AddVariableNode>> allVariableDefsMap;

    /**
     * Type name for struct.
     */
    std::string typeName;

    /**
     * Type name for parent.
     */
    std::string parentTypeName;

    /**
     * Store AddVariableNode so class can create all defined variables. We take
     * ownership of all nodes. There may be additional nodes that are not in this
     * vector and will be stored in a parent class.
     */
    std::vector<std::shared_ptr<class AddVariableNode>> variableDefs;

    /**
     * We activate the definition once evaluate is called. This is when we can
     * try to locate the definition of the parent if there is one and install
     * it in the scope. We also refuse to allow more than one definition in the
     * scope as you would expect so evaluate() cannot be called more than once!
     */
    bool active{false};
};