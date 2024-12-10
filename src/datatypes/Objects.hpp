//
//  Objects.hpp
//  Eucleia
//
//  Created by Edward on 14/01/2024.
//

#pragma once
#include "ArrayClassObject.hpp"
#include "ArrayObject.hpp"
#include "BaseObject.hpp"
#include "ClassDefinitionObject.hpp"
#include "ClassObject.hpp"
#include "EucleiaUtility.hpp"
#include "FloatObject.hpp"
#include "FunctionObject.hpp"
#include "IntObject.hpp"
#include "LibraryFunctionObject.hpp"
#include "StringObject.hpp"
#include "StructDefinitionObject.hpp"
#include "StructObject.hpp"
#include <ostream>

inline std::ostream &operator<<(std::ostream &out, const BaseObject &object)
{
    if (object.isObjectType<IntObject>())
        return (out << object.castObject<IntObject>().value);
    else if (object.isObjectType<FloatObject>())
        return (out << object.castObject<FloatObject>().value);
    else if (object.isObjectType<StringObject>())
        return (out << object.castObject<StringObject>().value);
    else if (object.isObjectType<ArrayObject>())
    {
        auto &arrayObject = object.castObject<ArrayObject>();

        out << "[";
        for (int i = 0; i < arrayObject.values.size(); i++)
        {
            out << *arrayObject[i] << ", ";
        }
        out << "]";
        return out;
    }
    else if (object.isObjectType<ArrayClassObject>())
    {
        auto &arrayClassObject = object.castObject<ArrayClassObject>();

        out << "[";
        for (BaseObject *object : arrayClassObject.objects)
        {
            out << (*object) << ", ";
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
            BaseObject *obj = structObject.instanceScope.getNamedObject(name);
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
            BaseObject *obj = classObject.instanceScope.getNamedObject(name);
            out << name << ": " << (*obj) << ", ";
        }
        out << ")";
        return out;
    }
    else
        return out; // Don't print anything.
}
