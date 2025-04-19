/**
 * @file AddArrayNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "ArrayObject.hpp"
#include "ProgramNode.hpp"
#include <vector>

class FileParser;


class AddArrayNode : public ProgramNode
{
public:
    AddArrayNode(std::vector<BaseNode *> nodes_) : ProgramNode(std::move(nodes_)) {}

    // [1, 2, 3, 4] OR [true, false, true] OR [1.2, 2.4] OR ["hello, ", "world!"].
    static AddArrayNode *parse(FileParser &parser);

    // Create a new ArrayObject in the current scope and return a managed pointer to it.
    ArrayObject *evaluate(Scope &scope) override;
};
