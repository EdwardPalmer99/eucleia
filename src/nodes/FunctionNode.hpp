/**
 * @file FunctionNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseObject.hpp"
#include "FunctionCallNode.hpp"
#include "ProgramNode.hpp"

class FileParser;


class FunctionNode : public FunctionCallNode
{
public:
    FunctionNode(BaseNode *funcName_, BaseNode *funcArgs_, BaseNode *funcBody_)
        : FunctionCallNode(funcName_, funcArgs_),
          funcBody(static_cast<ProgramNode *>(funcBody_))
    {
    }

    ~FunctionNode() override
    {
        delete funcBody;
    }

    BaseObject *evaluate(Scope &scope) override;

    /**
     * Parse:
     *
     * func funcName(int a, float b)
     * {
     *      body;
     * }
     */
    static FunctionNode *parse(FileParser &parser);

    ProgramNode *funcBody{nullptr};
};
