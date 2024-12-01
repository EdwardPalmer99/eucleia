/**
 * @file ClassDefinitionObject.hpp
 * @author Edward Palmer
 * @date 2024-11-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseObject.hpp"
#include "FunctionNode.hpp"
#include "StructDefinitionObject.hpp"
#include <string>
#include <vector>

/**
 * This class defines the format of a class and the variables and methods stored
 * inside. It will be stored in the scope along with the class name. We can then
 * use this to construct class instances.
 */
class ClassDefinitionObject : public StructDefinitionObject
{
public:
    /**
     * Supply vectors of nodes containing the variables and methods.
     */
    ClassDefinitionObject(std::vector<AddVariableNode *> variableDefs_,
                          std::vector<FunctionNode *> methodDefs_)
        : StructDefinitionObject(std::move(variableDefs_)),
          methodDefs(std::move(methodDefs_)) {}

    /**
     * Destructor deletes all method definition nodes.
     */
    ~ClassDefinitionObject() override;

    /**
     * Stores FunctionNodes. We will call evaluate() method to add them to class scope.
     * Note we are responsible for deleting these.
     */
    std::vector<FunctionNode *> methodDefs;
};