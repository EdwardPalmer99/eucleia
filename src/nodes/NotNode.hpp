/**
 * @file NotNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseObject.hpp"
#include "BaseUnaryOperatorNode.hpp"
#include "EucleiaScope.hpp"

/// Unary not operator (e.g. !true --> false).
class NotNode : public BaseUnaryOperatorNode
{
public:
    NotNode(BaseNode *body_)
        : BaseUnaryOperatorNode(body_)
    {
    }

    BaseObject *evaluate(Scope &scope) override;
};