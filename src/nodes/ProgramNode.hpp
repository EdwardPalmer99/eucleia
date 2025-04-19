/**
 * @file ProgramNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseNode.hpp"
#include "BaseObject.hpp"
#include "Scope.hpp"
#include <cassert>
#include <vector>

class FileParser;

/* TODO: - rename to BlockNode */
class ProgramNode : public BaseNode
{
public:
    ProgramNode(std::vector<BaseNode *> nodes_) : programNodes(std::move(nodes_)) {}

    ~ProgramNode() override
    {
        for (BaseNode *node : programNodes) // Call destructor on all nodes we own.
        {
            delete node;
        }
    }

    // Ownership of nodes passes to the caller.
    std::vector<BaseNode *> releaseNodes()
    {
        auto returnedNodes = programNodes;
        programNodes.clear();

        return returnedNodes;
    }

    BaseNode *operator[](size_t index) const
    {
        assert(index < programNodes.size());
        return programNodes[index];
    }

    // Evaluates a vector of nodes sequentially. Returns nullptr.
    BaseObject *evaluate(Scope &scope) override;

    /*
     * Parse a program (block) of code (i.e. inside { ...  }). If singleExpr is set to true, then if we have a single
     * expression in the block, we return that rather than a program node
     */
    static BaseNode *parse(FileParser &parser, bool singleExpr = true);

    std::vector<BaseNode *> programNodes;
};