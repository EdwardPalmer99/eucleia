/**
 * @file StructNode.hpp
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
 * Create a new empty StructObject. We expect that there exists a StructDefinitionObject
 * defined in the scope earlier from which to use to construct the StructObject.
 */
class StructNode : public BaseNode
{
public:
    explicit StructNode(std::string typeName_, std::string name_)
        : typeName(std::move(typeName_)), name(std::move(name_)) {}

    // Lookup in scope for StructDefinitionObject. Use that to construct an empty
    // StructObject.
    BaseObject *evaluate(Scope &scope) override;

    std::string name;
    std::string typeName;
};


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
    }

    // Returns object directly (use to set value).
    BaseObject *evaluateNoClone(Scope &scope) override;

    // Returns copy of object (getter).
    BaseObject *evaluate(Scope &scope) override;

    std::string instanceName;
    std::string memberName;
};