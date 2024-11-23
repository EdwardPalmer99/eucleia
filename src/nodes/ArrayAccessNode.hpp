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
#include "AddNewVariableNode.hpp"
#include "BaseNode.hpp"
#include "BaseObject.hpp"

class ArrayAccessNode : public BaseNode
{
public:
    ArrayAccessNode(LookupVariableNode *array_, AddIntNode *index_)
        : arrayLookup(array_),
          index(index_)
    {
    }

    ~ArrayAccessNode() override;

    // Returns copy of object at specified index in array. This makes sense as
    // imagine int a = integerArray[0], a += 1 --> integerArray[0] unchanged.
    BaseObject *evaluate(Scope &scope) override;

    LookupVariableNode *arrayLookup{nullptr};
    AddIntNode *index{nullptr};
};