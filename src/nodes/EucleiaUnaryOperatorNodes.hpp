//
//  EucleiaUnaryOperatorNodes.hpp
//  eucleia
//
//  Created by Edward on 04/02/2024.
//

#ifndef EucleiaUnaryOperatorNodes_hpp
#define EucleiaUnaryOperatorNodes_hpp

#include "EucleiaNode.hpp"
#include <memory>

using SharedPtr = std::shared_ptr<BaseObject>;

/// Base node for unary operators.
struct UnaryOperatorBaseNode : BaseNode
{
    UnaryOperatorBaseNode(SharedNode _body)
        : body{std::move(_body)}
    {
    }

    SharedNode body{nullptr};
};


/// Unary not operator (e.g. !true --> false).
struct NotNode : UnaryOperatorBaseNode
{
    NotNode(SharedNode _body)
        : UnaryOperatorBaseNode(std::move(_body))
    {
    }

    inline NodeType type() const override
    {
        return NodeType::Not;
    }

    BaseObject *evaluate(Scope &scope) override;
};


/// Unary - operator (.e.g. -1).
struct NegationNode : UnaryOperatorBaseNode
{
    NegationNode(SharedNode _body)
        : UnaryOperatorBaseNode(std::move(_body))
    {
    }

    inline NodeType type() const override
    {
        return NodeType::Negation;
    }

    BaseObject *evaluate(Scope &scope) override;
};


/// Unary prefix increment node (e.g. ++a).
struct PrefixIncrementNode : UnaryOperatorBaseNode
{
    PrefixIncrementNode(SharedNode _body)
        : UnaryOperatorBaseNode(std::move(_body))
    {
    }

    inline NodeType type() const override
    {
        return NodeType::PrefixIncrement;
    }

    BaseObject *evaluate(Scope &scope) override;
};


/// Unary prefix decrement node (e.g. --a).
struct PrefixDecrementNode : UnaryOperatorBaseNode
{
    PrefixDecrementNode(SharedNode _body)
        : UnaryOperatorBaseNode(std::move(_body))
    {
    }

    inline NodeType type() const override
    {
        return NodeType::PrefixDecrement;
    }

    BaseObject *evaluate(Scope &scope) override;
};


#endif /* EucleiaUnaryOperatorNodes_hpp */
