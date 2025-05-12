/**
 * @file BaseNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseObject.hpp"
#include "BaseObjectT.hpp"
#include "Scope.hpp"
#include <memory>
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
    Module
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

    virtual BaseObject *evaluate(Scope &scope) = 0;

    /* Evaluates object */
    template <typename TObject>
    TObject *evaluate(Scope &scope)
    {
        return static_cast<TObject *>(evaluate(scope));
    }

    /* Evaluates object and returns the object's stored value directly */
    template <typename TValue>
    TValue &evaluateObject(Scope &scope)
    {
        /* BaseObject ptr */
        BaseObject *baseObjPtr = evaluate(scope);

        /* Up-cast */
        auto &upcastedObj = static_cast<BaseObjectT<TValue> &>(*baseObjPtr);

        /* Apply operator overload to get stored value */
        return (*upcastedObj);
    }

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

using BaseNodeSharedPtrVector = std::vector<BaseNode::Ptr>;
using BaseNodePtrVector = std::vector<BaseNode *>;

/* Utility convert method */
BaseNodeSharedPtrVector toSharedNodePtrVector(BaseNodePtrVector &nodes);
