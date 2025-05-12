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


class BaseNode
{
public:
    using Ptr = std::shared_ptr<BaseNode>;


    BaseNode() = default; // TODO: - should be protected! Should not create BaseNode directly.
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


    template <class TNode>
    bool isNodeType() const
    {
        return typeid(*this) == typeid(TNode);
    }


    bool typesMatch(const BaseNode &other) const
    {
        return typeid(*this) == typeid(other);
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
};

using BaseNodeSharedPtrVector = std::vector<BaseNode::Ptr>;
using BaseNodePtrVector = std::vector<BaseNode *>;

/* Utility convert method */
BaseNodeSharedPtrVector toSharedNodePtrVector(BaseNodePtrVector &nodes);
