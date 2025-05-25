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


AnyObject::Type AnyObject::getUserObjectType(const std::string &name)
{
    /* TODO: - use a static map to make more efficient */


    /* Todo: enable also for functions, etc */
    if (name == "int")
        return AnyObject::Int;
    else if (name == "bool")
        return AnyObject::Bool;
    else if (name == "float")
        return AnyObject::Float;
    else if (name == "string")
        return AnyObject::String;
    else if (name == "array")
        return AnyObject::Array;
    else if (name == "struct")
        return AnyObject::Struct; /* TODO: -  add */
    else if (name == "class")
        return AnyObject::Class;

    ThrowException("'" + name + "' is not a valid user object type");
}


std::string AnyObject::typeToString(Type type)
{
    static const std::unordered_map<Type, std::string> TypeToString = {{NotSet, "NotSet"},
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

    ThrowException("Unknown AnyObjectType!");
}


std::string AnyObject::typeToString() const
{
    return AnyObject::typeToString(_type);
}


AnyObject &AnyObject::operator=(const AnyObject &other)
{
    /* TODO: - implement if required */
    AnyObject::Vector cloneVector(const AnyObject::Vector &);

    /* Handling not-set return type */
    if (getType() == AnyObject::NotSet || other.getType() == AnyObject::NotSet)
    {
        _value = other._value;
        _type = other._type;
        return (*this);
    }

    if (getType() != other.getType()) /* TODO: - bit hacky. But return type can take any */
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


AnyObject AnyObject::clone() const
{
    AnyObject::Vector cloneVector(const AnyObject::Vector &);

    switch (getType())
    {
        case Bool:
            return AnyObject(getValue<bool>()); /* TODO: - these are implicit. All nodes with evaluateRef implmemeneted can call it with evaluate() and just copy */
        case Int:
            return AnyObject(getValue<long>()); /* TODO: - don't need any of these except vector */
        case Float:
            return AnyObject(getValue<double>());
        case String:
            return AnyObject(getValue<std::string>());
        case Array:
            return AnyObject(cloneVector(getValue<Vector>()));
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
        clone.push_back(object.clone()); /* Deep-copy */
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
        out << object << ", ";
    }

    out << "]";

    return out;
}