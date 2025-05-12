/**
 * @file StructAccessNode.hpp
 * @author Edward Palmer
 * @date 2024-11-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseNode.hpp"
#include "BaseObject.hpp"
#include "BasePropertyNode.hpp"
#include "Scope.hpp"
#include <string>

/**
 * Returns object in a defined struct instance.
 *
 * [structName].[memberVariableName]
 */
class StructAccessNode : public BasePropertyNode
{
public:
    explicit StructAccessNode(std::string instanceName_,
                              std::string memberName_)
        : instanceName(std::move(instanceName_)),
          memberName(std::move(memberName_))
    {
      setType(NodeType::StructAccess);
    }

    // Returns object directly (use to set value).
    BaseObject *evaluateNoClone(Scope &scope) override;

    // Returns copy of object (getter).
    BaseObject *evaluate(Scope &scope) override;

    std::string instanceName;
    std::string memberName;
};