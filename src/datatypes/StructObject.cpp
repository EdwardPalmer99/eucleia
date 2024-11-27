/**
 * @file StructObject.cpp
 * @author Edward Palmer
 * @date 2024-11-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "StructObject.hpp"
#include "FloatObject.hpp"
#include "IntObject.hpp"
#include "StringObject.hpp"
#include <cassert>

StructObject &StructObject::operator=(const BaseObject &other)
{
    if (this == &other)
    {
        return (*this);
    }

    // 1. cast to struct object.
    const StructObject &otherStruct = other.castObject<StructObject>();

    // 2. Are the struct objects equal? We need to know that they have the same
    // struct template. Not ideal but we can iterate through and compare the typeids
    // of each object in the maps.
    if (otherStruct.objectForName.size() != objectForName.size())
    {
        EucleiaError("%s\n", "attempting to assign different struct types.");
    }

    for (auto &[name, object] : objectForName)
    {
        auto iter = otherStruct.objectForName.find(name);
        if (iter == otherStruct.objectForName.end())
        {
            EucleiaError("%s\n", "attempting to assign different struct types.");
        }

        // Attempt an assignment. Will fail if different types.
        *object = *(iter->second);
    }

    return (*this);
}

StructObject::StructObject(StructDefinitionObject *structDefinitionObject)
{
    assert(structDefinitionObject);

    // Build our map of empty object from the definition. Note that this struct
    // will have ownership of these objects rather than the scope!
    for (auto &[name, objectType] : structDefinitionObject->memberVariableTypes)
    {
        switch (objectType) // TODO: - need factory to create objects for type.
        {
            case StructDefinitionObject::SupportedObjectTypes::IntObject:
                objectForName[name] = new IntObject();
                break;
            case StructDefinitionObject::SupportedObjectTypes::FloatObject:
                objectForName[name] = new FloatObject();
                break;
            case StructDefinitionObject::SupportedObjectTypes::StringObject:
                objectForName[name] = new StringObject();
                break;
            default:
                EucleiaError("%s\n", "unsupported object type for struct instance.");
                break;
        }
    }
}


StructObject::~StructObject()
{
    // Delete allocated objects.
    for (auto iter = objectForName.begin(); iter != objectForName.end(); ++iter)
    {
        delete iter->second;
        iter->second = nullptr;
    }

    objectForName.clear();
}