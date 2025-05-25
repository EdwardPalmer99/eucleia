/**
 * @file LookupVariableNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseNode.hpp"
#include "Scope.hpp"
#include <string>

class LookupVariableNode : public BaseNode
{
public:
    LookupVariableNode(std::string name) : _name(std::move(name))
    {
        setType(NodeType::LookupVariable);
    }

    [[nodiscard]] inline const std::string &name() const;

    /* Returns the object in the scope associated with a variable name */
    class AnyObject &evaluateRef(Scope &scope) override;
    class AnyObject evaluate(Scope &scope) override;

private:
    std::string _name;
};


const std::string &LookupVariableNode::name() const
{
    return _name;
}
