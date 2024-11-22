/**
 * @file AddStringNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseNode.hpp"
#include "StringObject.hpp"
#include <string>

class AddStringNode : public BaseNode
{
public:
    AddStringNode(std::string string_) : stringObject(std::move(string_)) {}

    // Creates a StringObject in the scope and returns managed pointer to it.
    StringObject *evaluate(Scope &scope) override;


    StringObject stringObject;
};
