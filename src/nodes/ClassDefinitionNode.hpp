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
#include "StructDefinitionNode.hpp"
#include <memory>
#include <string>
#include <vector>


/**
 * This class defines the format of a class and the variables and methods stored
 * inside. It will be stored in the scope along with the class name. We can then
 * use this to construct class instances.
 */
class ClassDefinitionNode : public StructDefinitionNode
{
public:
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

protected:
    /**
     * Builds the method map from parent classes.
     */
    void buildMethodDefsHashMap(const Scope &scope);

    /**
     * Stores FunctionNodes. We will call evaluate() method to add them to class scope.
     * Note we are responsible for deleting these.
     */
    std::vector<FunctionNode::Ptr> methodDefs;

    /**
     * Stores our owned methods and those of any parent methods we inherit. If
     * we have a method with the same name as one in a parent class, we will
     * "override" it by using our own.
     */
    std::unordered_map<std::string, FunctionNode::Ptr> allMethodDefsMap;
};