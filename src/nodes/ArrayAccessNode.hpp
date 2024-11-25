/**
 * @file ArrayAccessNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "AddIntNode.hpp"
#include "AddVariableNode.hpp"
#include "BaseObject.hpp"
#include "BasePropertyNode.hpp"
#include "Scope.hpp"

class ArrayAccessNode : public BasePropertyNode
{
public:
    ArrayAccessNode(LookupVariableNode *array_, AddIntNode *index_)
        : arrayLookup(array_),
          index(index_)
    {
    }

    ~ArrayAccessNode() override;

    // Returns object directly.
    BaseObject *objectPtr(Scope &scope) override;

    // Returns copy of object.
    BaseObject *evaluate(Scope &scope) override;

    // Updates value of object.
    void setObject(Scope &scope, BaseObject *object) override;

    LookupVariableNode *arrayLookup{nullptr};
    AddIntNode *index{nullptr};
};