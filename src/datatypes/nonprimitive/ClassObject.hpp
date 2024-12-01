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
#include "StructObject.hpp"


/**
 * An instance of a class defined by the ClassDefinitionObject.
 */
class ClassObject : public StructObject
{
public:
    explicit ClassObject(ClassDefinitionObject *classDefinition_);

    ~ClassObject() override;
};