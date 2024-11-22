/**
 * @file FileNode.cpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "FileNode.hpp"

BaseObject *FileNode::evaluate(Scope &globalScope)
{
    for (const auto &node : programNodes)
    {
        node->evaluate(globalScope);
    }

    return nullptr;
}