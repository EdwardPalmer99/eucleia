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


class BaseNode
{
public:
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

    template <typename T>
    T *evaluate(Scope &scope)
    {
        return static_cast<T *>(evaluate(scope));
    }

    /* Returns a non-const reference to the stored value */
    // template <typename TValue>
    // TValue &evaluate(Scope &scope)
    // {
    //     auto obj = evaluate(scope);

    //     /* Cast and apply '*' operator */
    //     return (*static_cast<BaseObjectT<TValue>>(obj);
    // }
};