/**
 * @file ClassObject.hpp
 * @author Edward Palmer
 * @date 2024-11-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "ClassDefinitionObject.hpp"
#include "Exceptions.hpp"
#include "StructObject.hpp"
#include <string>

/**
 * An instance of a class defined by the ClassDefinitionObject.
 */
class ClassObject : public StructObject
{
public:
    ClassObject(std::string typeName_, std::string name_);

    ClassObject &operator=(const BaseObject &) override
    {
        ThrowException("Not implemented");
    }

    /**
     * Finishes initializing the class object and links to the scope.
     * @param scope The scope in which to add the instance.
     * @return BaseObject* Pointer to itself
     */
    BaseObject *evaluate(Scope &scope) override;

    friend class ClassMethodCallNode;
};