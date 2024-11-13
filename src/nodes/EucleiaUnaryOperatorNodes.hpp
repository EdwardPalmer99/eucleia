//
//  EucleiaUnaryOperatorNodes.hpp
//  eucleia
//
//  Created by Edward on 04/02/2024.
//

#ifndef EucleiaUnaryOperatorNodes_hpp
#define EucleiaUnaryOperatorNodes_hpp

#include "EucleiaNode.hpp"
#include "EucleiaObject.hpp"
#include "EucleiaScope.hpp"

/// Base node for unary operators.
class UnaryOperatorBaseNode : public BaseNode
{
public:
    UnaryOperatorBaseNode(BaseNode *body_)
        : body(body_)
    {
    }

    BaseNode *body{nullptr};
};


/// Unary not operator (e.g. !true --> false).
class NotNode : public UnaryOperatorBaseNode
{
public:
    NotNode(BaseNode *body_)
        : UnaryOperatorBaseNode(body_)
    {
    }

    BaseObject *evaluate(Scope &scope) override;
};


/// Unary - operator (.e.g. -1).
class NegationNode : public UnaryOperatorBaseNode
{
public:
    NegationNode(BaseNode *body_)
        : UnaryOperatorBaseNode(body_)
    {
    }

    BaseObject *evaluate(Scope &scope) override;
};


/// Unary prefix increment node (e.g. ++a).
class PrefixIncrementNode : public UnaryOperatorBaseNode
{
public:
    PrefixIncrementNode(BaseNode *body_)
        : UnaryOperatorBaseNode(body_)
    {
    }

    BaseObject *evaluate(Scope &scope) override;
};


/// Unary prefix decrement node (e.g. --a).
class PrefixDecrementNode : public UnaryOperatorBaseNode
{
public:
    PrefixDecrementNode(BaseNode *body_)
        : UnaryOperatorBaseNode(body_)
    {
    }

    BaseObject *evaluate(Scope &scope) override;
};


#endif /* EucleiaUnaryOperatorNodes_hpp */
