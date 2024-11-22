/**
 * @file FileNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseObject.hpp"
#include "EucleiaScope.hpp"
#include "ProgramNode.hpp"
#include <vector>

class FileNode : public ProgramNode
{
public:
    FileNode(std::vector<BaseNode *> nodes_ = {}) : ProgramNode(std::move(nodes_)) {}

    // Evaluates a file treating it as one large program. Does NOT create an inner
    // scope node in order to ensure that any functions declared in this file will
    // be added to the master file by using the same global scope - TODO: - think about this logic.
    BaseObject *evaluate(Scope &globalScope) override;
};