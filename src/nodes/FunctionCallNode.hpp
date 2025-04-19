/**
 * @file FunctionCallNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "AddVariableNode.hpp"
#include "BaseNode.hpp"
#include "BaseObject.hpp"
#include "ProgramNode.hpp"
#include "Scope.hpp"

class FileParser;

class FunctionCallNode : public BaseNode
{
public:
    FunctionCallNode(BaseNode *funcName_, BaseNode *funcArgs_)
        : funcName(static_cast<AddVariableNode *>(funcName_)),
          funcArgs(static_cast<ProgramNode *>(funcArgs_))
    {
    }

    ~FunctionCallNode() override
    {
        delete funcName;
        delete funcArgs;
    }

    // TODO: - don't forget to do performance profiling for Fib sequence and see memory requirements for old and new version
    // TODO: - create a new PR after this for parser to store all nodes in AST in flat array using pointers with method to delete by walking along array.
    BaseObject *evaluate(Scope &scope) override;

    BaseObject *evaluateFunctionBody(BaseNode &funcBody, Scope &funcScope);

    /* Parse: funcName(arg1, arg2, ...) */
    static FunctionCallNode *parse(FileParser &parser, BaseNode *funcName);

    AddVariableNode *funcName{nullptr};
    ProgramNode *funcArgs{nullptr};
};
