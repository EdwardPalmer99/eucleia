/**
 * @file AddIntNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseNode.hpp"
#include "IntObject.hpp"
#include "Scope.hpp"
#include "Token.hpp"

class FileParser;

class AddIntNode : public BaseNode
{
public:
    AddIntNode(long int_) : intObject(int_) {}

    // Creates an IntObject in the scope and returns a managed pointer to it.
    IntObject *evaluate(Scope &scope) override;

    static AddIntNode *parse(FileParser &parser);

    IntObject intObject;
};
