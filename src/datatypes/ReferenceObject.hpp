/**
 * @file ReferenceObject.hpp
 * @author Edward Palmer
 * @date 2025-01-01
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "BaseObject.hpp"
#include "EucleiaUtility.hpp"

// TODO: - issues;
// 1. object we are linked to goes out of scope and is deleted.
// 2. link to different object.
// 3. set to null.

// TODO: - needs to have an object type for the reference.
// TODO: - think about scope and what happens when linkedObject is destroyed.
/**
 * @brief Maintains a reference to another object.
 */
class ReferenceObject : public BaseObject
{
public:
    ReferenceObject(BaseObject *linkedObject_ = nullptr) : linkedObject(linkedObject_) {}

    /**
     * Set the value of the linked object.
     */
    BaseObject &operator=(const BaseObject &other) override
    {
        if (!linkedObject)
        {
            EucleiaError("No object linked to reference.");
        }

        if (linkedObject != &other)
        {
            *linkedObject = other; // Set.
        }

        return (*linkedObject);
    }

    /**
     * Clone the object we are linked to and return an error if not linked.
     */
    BaseObject *clone() const override;

    /**
     * Object we are linked to.
     */
    BaseObject *linkedObject{nullptr};
};
