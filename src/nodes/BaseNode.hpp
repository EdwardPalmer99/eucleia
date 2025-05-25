/**
 * @file BaseNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "Exceptions.hpp"
#include "Scope.hpp"
#include <memory>
#include <optional>
#include <vector>


enum class NodeType
{
    Unknown = (-1),
    Bool,
    Int,
    String,
    Float,
    If,
    ForLoop,
    Break,
    Return,
    Not,
    While,
    DoWhile,
    Block,
    Assign,
    Array,
    File,
    PrefixIncrement,
    PrefixDecrement,
    Negation,
    Binary,
    Program,
    StructAccess,
    StructDefinition,
    FunctionCall,
    LookupVariable,
    Function,
    ClassDefinition,
    ClassMethodCall,
    ArrayAccess,
    AddVariable,
    Module,
    Cast
};


class BaseNode
{
public:
    using Ptr = std::shared_ptr<BaseNode>;

    virtual ~BaseNode() = default;

    template <class TNode>
    const TNode &castNode() const
    {
        return static_cast<TNode &>(*this);
    }


    template <class TNode>
    TNode &castNode()
    {
        return static_cast<TNode &>(*this);
    }


    bool isNodeType(NodeType queryType) const
    {
        return (type() == queryType);
    }


    bool typesMatch(const BaseNode &other) const
    {
        return type() == other.type();
    }

    /* Returns a copy of an object (good for light-weight types) */
    virtual class AnyObject evaluate(Scope &); // TODO: - can this be const-cast?


    virtual class AnyObject &evaluateRef(Scope &);

    void setType(NodeType type)
    {
        _type = type;
    }

    NodeType type() const { return _type; } /* TODO: - move into base class */

protected:
    BaseNode() = default;
    BaseNode(NodeType type) : _type(type) {}

    NodeType _type{NodeType::Unknown};
};

using BaseNodePtrVector = std::vector<BaseNode::Ptr>;
