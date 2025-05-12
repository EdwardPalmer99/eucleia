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
#include <string>

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

    std::string instanceName;
    FunctionCallNode *methodCallNode;
};
