/**
 * @file NegationNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseUnaryOperatorNode.hpp"

/// Unary - operator (.e.g. -1).
class NegationNode : public BaseUnaryOperatorNode
{
public:
    NegationNode(BaseNode *body_)
        : BaseUnaryOperatorNode(body_)
    {
    }

    BaseObject *evaluate(Scope &scope) override;
};