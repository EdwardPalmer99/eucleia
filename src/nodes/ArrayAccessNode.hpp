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

// TODO: - rewrite this...
class ArrayAccessNode : public BaseNode
{
public:
    ArrayAccessNode(BaseNode *arrayName_, BaseNode *arrayIndex_)
        : arrayName(static_cast<AddNewVariableNode *>(arrayName_)),
          arrayIndex(static_cast<AddIntNode *>(arrayIndex_))
    {
    }

    ~ArrayAccessNode() override
    {
        delete arrayName;
        delete arrayIndex;
    }

    BaseObject *evaluate(Scope &scope) override;

    AddNewVariableNode *arrayName{nullptr};
    AddIntNode *arrayIndex{nullptr};
};