/**
 * @file StructObject.hpp
 * @author Edward Palmer
 * @date 2024-11-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseObject.hpp"
#include "EucleiaUtility.hpp"
#include "Scope.hpp"
#include "StructDefinitionObject.hpp"
#include <string>
#include <unordered_set>

/**
 * struct SomeStruct a;
 */
class StructObject : public BaseObject
{
public:
    StructObject() = delete;

    /**
     * Construct an instance of a struct from a definition.
     */
    StructObject(StructDefinitionObject *structDefinition_);

    /**
     *
     */
    StructObject &operator=(const BaseObject &other) override;

    // TODO: - implement in future.
    StructObject *clone() const override
    {
        EucleiaError("not implemented!");
    }

    /**
     * The struct has its own scope for storing its own variables. It does not
     * inherit from any parent scopes. Literally just used for storing stuff.
     * When this instance goes out of scope, all variables will be deleted.
     */
    Scope instanceScope;

    /**
     * Stores names of all variables stored in set.
     */
    std::unordered_set<std::string> variableNames;

    /**
     * Pointer to the struct definition. This will be stored in some parent scope.
     * We use this to check whether another instance shares the same class definition.
     */
    StructDefinitionObject *structDefinition;
};
