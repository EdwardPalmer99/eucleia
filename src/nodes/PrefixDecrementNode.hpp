/**
 * @file PrefixDecrementNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseObject.hpp"
#include "BaseUnaryOperatorNode.hpp"

/// Unary prefix decrement node (e.g. --a).
class PrefixDecrementNode : public BaseUnaryOperatorNode
{
public:
    PrefixDecrementNode(BaseNode *body_)
        : BaseUnaryOperatorNode(body_)
    {
    }

    BaseObject *evaluate(Scope &scope) override;
};
