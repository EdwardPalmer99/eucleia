//
//  Objects.hpp
//  Eucleia
//
//  Created by Edward on 14/01/2024.
//

#pragma once
#include "ArrayObject.hpp"
#include "BaseObject.hpp"
#include "ClassDefinitionObject.hpp"
#include "ClassObject.hpp"
#include "Exceptions.hpp"
#include "FloatObject.hpp"
#include "FunctionObject.hpp"
#include "IntObject.hpp"
#include "ModuleFunctionObject.hpp"
#include "StringObject.hpp"
#include "StructDefinitionObject.hpp"
#include "StructObject.hpp"
#include <ostream>

inline std::ostream &operator<<(std::ostream &out, const BaseObject &object)
{
    if (object.isObjectType<IntObject>())
        return (out << IntObject::value(object));
    else if (object.isObjectType<FloatObject>())
        return (out << FloatObject::value(object));
    else if (object.isObjectType<StringObject>())
        return (out << StringObject::value(object));
    else if (object.isObjectType<ArrayObject>())
    {
        auto &arrayObject = object.castObject<ArrayObject>();

        out << "[";
        for (int i = 0; i < arrayObject.value().size(); i++)
        {
            out << *arrayObject[i] << ", ";
        }
        out << "]";
        return out;
    }
    else if (object.isObjectType<StructObject>())
    {
        auto &structObject = object.castObject<StructObject>();

        out << "(";
        for (auto &name : structObject.variableNames)
        {
            BaseObject::Ptr obj = structObject._instanceScope.getNamedObject(name);
            out << name << ": " << (*obj) << ", ";
        }
        out << ")";
        return out;
    }
    else if (object.isObjectType<ClassObject>())
    {
        auto &classObject = object.castObject<ClassObject>();

        out << "(";
        for (auto &name : classObject.variableNames)
        {
            BaseObject::Ptr obj = classObject._instanceScope.getNamedObject(name);
            out << name << ": " << (*obj) << ", ";
        }
        out << ")";
        return out;
    }
    else
        return out; // Don't print anything.
}
