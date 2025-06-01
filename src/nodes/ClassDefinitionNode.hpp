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
#include "FunctionNode.hpp"
#include "Scope.hpp"
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>


/**
 * This class defines the format of a class and the variables and methods stored
 * inside. It will be stored in the scope along with the class name. We can then
 * use this to construct class instances.
 */
class ClassDefinitionNode : public BaseNode, public std::enable_shared_from_this<ClassDefinitionNode>
{
public:
    using Ptr = std::shared_ptr<ClassDefinitionNode>;

    /**
     * Supply vectors of nodes containing the variables and methods. We only
     * pass vectors to the variables and methods we actually own. The others
     * will be in a parent class if provided.
     */
    ClassDefinitionNode(std::string typeName_,
                        std::string parentTypeName_,
                        std::vector<std::shared_ptr<AddVariableNode>> variableDefs_,
                        std::vector<std::shared_ptr<FunctionNode>> methodDefs_);

    /**
     * Destructor deletes all method definition nodes.
     */
    ~ClassDefinitionNode() override = default;

    /**
     * Install object in current scope.
     */
    std::shared_ptr<class AnyObject> evaluate(Scope &scope) override;

    /**
     * Calls evaluate method on all method nodes. Installs them in argument scope.
     */
    void installMethodsInScope(Scope &scope) const;

    /**
     * Calls evaluate method on all variables in this struct and parents. Installs them in argument scope.
     */
    void installVariablesInScope(Scope &scope) const;

protected:
    /**
     * Builds the method map from parent classes.
     */
    void buildMethodDefsHashMap(const Scope &scope);

    /**
     * Builds the variableDefsMap from any parent classes and checks for clashing variables.
     */
    void buildVariableDefHashMap(const Scope &scope);

    /**
     * Returns a pointer to the parent struct or nullptr if not found.
     */
    std::shared_ptr<ClassDefinitionNode> lookupParent(const Scope &scope) const;

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

    /**
     * Stores FunctionNodes. We will call evaluate() method to add them to class scope.
     * Note we are responsible for deleting these.
     */
    std::vector<FunctionNode::Ptr> methodDefs;

    /**
     * Stores our owned variables and those of any parent variables we inherit.
     * To construct the object, we will call evaluate() method on each node.
     */
    std::unordered_map<std::string, std::shared_ptr<class AddVariableNode>> allVariableDefsMap;

    /**
     * Stores our owned methods and those of any parent methods we inherit. If
     * we have a method with the same name as one in a parent class, we will
     * "override" it by using our own.
     */
    std::unordered_map<std::string, FunctionNode::Ptr> allMethodDefsMap;
};