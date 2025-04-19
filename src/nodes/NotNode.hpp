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
#include "Scope.hpp"

class FileParser;


/// Unary not operator (e.g. !true --> false).
class NotNode : public BaseUnaryOperatorNode
{
public:
    NotNode(BaseNode *body_)
        : BaseUnaryOperatorNode(body_)
    {
    }

    BaseObject *evaluate(Scope &scope) override;

    static NotNode *parse(FileParser &parser);
};