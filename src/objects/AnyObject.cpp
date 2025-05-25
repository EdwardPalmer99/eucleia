/**
 * @file AnyObject.cpp
 * @author Edward Palmer
 * @date 2025-05-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "AnyObject.hpp"
#include "Exceptions.hpp"
#include "StructNode.hpp"


AnyObject::Type AnyObject::stringToType(const std::string &name)
{
    static const StringToTypeHashMap StringToType = {{"int", AnyObject::Int},
                                                     {"float", AnyObject::Float},
                                                     {"bool", AnyObject::Bool},
                                                     {"string", AnyObject::String},
                                                     {"array", AnyObject::Array},
                                                     {"struct", AnyObject::Struct},
                                                     {"class", AnyObject::Class}};

    auto iter = StringToType.find(name);
    if (iter != StringToType.end())
    {
        return (iter->second);
    }

    ThrowException("Type [" + name + "] is not a valid user type");
};


std::string AnyObject::typeToString(AnyObject::Type type)
{
    static const TypeToStringHashMap TypeToString = {{NotSet, "NotSet"},
                                                     {Int, "Int"},
                                                     {Bool, "Bool"},
                                                     {Float, "Float"},
                                                     {String, "String"},
                                                     {Array, "Array"},
                                                     {Struct, "Struct"},
                                                     {Class, "Class"},
                                                     {_UserFunction, "Function"},
                                                     {_ModuleFunction, "ModuleFunction"},
                                                     {_StructDefinition, "StructDef"},
                                                     {_ClassDefinition, "ClassDef"}};

    auto iter = TypeToString.find(type);
    if (iter != TypeToString.end())
    {
        return (iter->second);
    }

    ThrowException("Type [" + std::to_string(type) + "] is not valid");
}


std::string AnyObject::typeToString() const
{
    return AnyObject::typeToString(_type);
}


AnyObject &AnyObject::operator=(const AnyObject &other)
{
    /* TODO: - implement if required */
    AnyObject::Vector cloneVector(const AnyObject::Vector &);

    if (getType() != other.getType())
    {
        ThrowException("Invalid assignment. Types do not match [LHS = " + typeToString() + ", RHS = " + other.typeToString() + "]");
    }

    switch (getType())
    {
        case Int:
        case Bool:
        case Float:
        case String:
            _value = other._value; /* Standard copy assignment */
            break;
        case Struct:
            *std::static_pointer_cast<StructNode>(getValue<BaseNode::Ptr>()) = *std::static_pointer_cast<StructNode>(other.getValue<BaseNode::Ptr>());
            break;
        case Array: /* Array is a vector of shared pointers --> need to clone for deep-copy */
            getValue<Vector>() = cloneVector(other.getValue<Vector>());
            break;
        default:
            ThrowException("Copy assignment not implemented for object type [" + typeToString() + "]");
    }

    return (*this);
}


AnyObject::Ptr AnyObject::clone() const
{
    AnyObject::Vector cloneVector(const AnyObject::Vector &);

    switch (getType())
    {
        case Bool:
            return std::make_shared<AnyObject>(getValue<bool>());
        case Int:
            return std::make_shared<AnyObject>(getValue<long>());
        case Float:
            return std::make_shared<AnyObject>(getValue<double>());
        case String:
            return std::make_shared<AnyObject>(getValue<std::string>());
        case Array:
            return std::make_shared<AnyObject>(cloneVector(getValue<Vector>()));
        default:
            ThrowException("clone() is not implemented for object type [" + typeToString() + "]");
    }
}

AnyObject::Vector cloneVector(const AnyObject::Vector &vector)
{
    AnyObject::Vector clone;

    clone.reserve(vector.size());

    for (const auto &object : vector)
    {
        clone.push_back(object->clone()); /* Deep-copy */
    }

    return clone;
}

std::ostream &operator<<(std::ostream &out, const AnyObject::Vector &array);


std::ostream &operator<<(std::ostream &out, const AnyObject &object)
{
    std::string printBoolean(const bool &value);

    switch (object.getType())
    {
        case AnyObject::Bool:
            return (out << printBoolean(object.getValue<bool>()));
        case AnyObject::Int:
            return (out << object.getValue<long>());
        case AnyObject::Float:
            return (out << object.getValue<double>());
        case AnyObject::String:
            return (out << object.getValue<std::string>());
        case AnyObject::Array:
            return (out << object.getValue<AnyObject::Vector>());
        default:
            return out; /* Don't print anything --> not supported */
    };
}


std::string printBoolean(const bool &value)
{
    return (value ? "true" : "false");
}


std::ostream &operator<<(std::ostream &out, const AnyObject::Vector &array)
{
    out << "[";

    for (const auto &object : array)
    {
        out << *object << ", ";
    }

    out << "]";

    return out;
}