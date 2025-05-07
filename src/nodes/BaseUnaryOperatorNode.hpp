/**
 * @file BaseUnaryOperatorNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseNode.hpp"

/// Base node for unary operators.
class BaseUnaryOperatorNode : public BaseNode
{
public:
    BaseUnaryOperatorNode(BaseNode *body_)
        : body(body_)
    {
    }

    ~BaseUnaryOperatorNode() override
    {
        delete body;
    }

    BaseNode *body{nullptr};
};