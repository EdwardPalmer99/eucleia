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