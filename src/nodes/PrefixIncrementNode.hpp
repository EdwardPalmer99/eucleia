/**
 * @file PrefixIncrementNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "BaseObject.hpp"
#include "BaseUnaryOperatorNode.hpp"


/// Unary prefix increment node (e.g. ++a).
class PrefixIncrementNode : public BaseUnaryOperatorNode
{
public:
    PrefixIncrementNode(BaseNode *body_)
        : BaseUnaryOperatorNode(body_)
    {
    }

    BaseObject *evaluate(Scope &scope) override;
};