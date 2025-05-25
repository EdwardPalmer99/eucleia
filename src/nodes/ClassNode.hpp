/**
 * @file ClassNode.hpp
 * @author Edward Palmer
 * @date 2024-11-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "ClassDefinitionNode.hpp"
#include "Exceptions.hpp"
#include "StructNode.hpp"
#include <memory>
#include <string>

/**
 * An instance of a class defined by the ClassDefinitionNode.
 */
class ClassNode : public StructNode
{
public:
    ClassNode(std::string typeName_, std::string name_);

    // ClassNode &operator=(const BaseObject &) override
    // {
    //     ThrowException("Not implemented");
    // }

    /**
     * Finishes initializing the class object and links to the scope.
     * @param scope The scope in which to add the instance.
     * @return BaseObject* Pointer to itself
     */
    std::shared_ptr<class AnyObject> evaluate(Scope &scope) override;
};