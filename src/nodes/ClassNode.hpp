/**
 * @file ClassNode.hpp
 * @author Edward Palmer
 * @date 2024-12-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseNode.hpp"
#include "FunctionCallNode.hpp"
#include "Scope.hpp"
#include "StructNode.hpp"
#include <string>

/**
 * Create a new empty ClassObject. We expect there exists a ClassDefinitionObject
 * defined in the scope earlier.
 */
class ClassNode : public StructNode
{
public:
    ClassNode(std::string typeName_, std::string name_) : StructNode(typeName_, name_) {}

    BaseObject *evaluate(Scope &scope) override;
};


/**
 * Access class method.
 *
 * [class instance name].[method call]
 */
class ClassMethodCallNode : public BaseNode
{
public:
    ClassMethodCallNode(std::string instanceName_, FunctionCallNode *methodCallNode_);

    ~ClassMethodCallNode() override;

    BaseObject *evaluate(Scope &scope) override;

    FunctionCallNode *methodCallNode;
    std::string instanceName;
};
