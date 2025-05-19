/**
 * @file Object.hpp
 * @author Edward Palmer
 * @date 2025-05-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "BaseNode.hpp"
#include "ModuleFunctor.hpp"
#include <cassert>
#include <memory>
#include <ostream>
#include <string>
#include <variant>
#include <vector>


class AnyObject
{
public:
    using Ptr = std::shared_ptr<AnyObject>;
    using Vector = std::vector<AnyObject::Ptr>;

    virtual ~AnyObject() = default; /* In case we subclass */

    enum Type
    {
        NotSet = (-1),
        Int, /* User-defined types */
        Bool,
        Float,
        String,
        Array,
        Struct,
        Class,

        _UserFunction, /* Private implementation-types */
        _ModuleFunction,
        _StructDefinition,
        _ClassDefinition,
    };

    /* Converts user types like "int" --> AnyType::Int or returns None if not found */
    static AnyObject::Type getUserObjectType(const std::string &name);

    std::string typeToString() const;

    /* NB: require explicit to avoid implicit casting */
    explicit AnyObject(bool value) : _value(value), _type(Bool) {}
    explicit AnyObject(long value) : _value(value), _type(Int) {} /* NB: require explicit to avoid implicit casting */
    explicit AnyObject(double value) : _value(value), _type(Float) {}
    explicit AnyObject(std::string value) : _value(std::move(value)), _type(String) {}
    explicit AnyObject(AnyObject::Vector value) : _value(std::move(value)), _type(Array) {}
    explicit AnyObject(ModuleFunctor value) : _value(std::move(value)), _type(_ModuleFunction) {}

    /* _userFuntion, _StructDefinition, Struct, ...*/
    explicit AnyObject(std::shared_ptr<BaseNode> value, Type type) : _value(std::move(value)), _type(type) {}

    AnyObject &operator=(const AnyObject &other);

    template <typename TValue>
    [[nodiscard]] inline TValue &getValue();

    template <typename TValue>
    [[nodiscard]] inline const TValue &getValue() const;

    [[nodiscard]] inline Type getType() const;

    [[nodiscard]] inline bool isType(Type expectedType) const;

    [[nodiscard]] AnyObject::Ptr clone() const;

    friend std::ostream &operator<<(std::ostream &out, const AnyObject &object);

protected:
    AnyObject() = default; /* Prevent direct initialization */

private:
    using ValueVariant = std::variant<long,
                                      bool,
                                      double,
                                      std::string,
                                      Vector,
                                      ModuleFunctor,
                                      BaseNode::Ptr>;
    ValueVariant _value{};
    Type _type{Type::NotSet};
};


template <typename TValue> // No type checking!!
TValue &AnyObject::getValue()
{
    return std::get<TValue>(_value);
}


template <typename TValue>
const TValue &AnyObject::getValue() const
{
    return std::get<TValue>(_value);
}


AnyObject::Type AnyObject::getType() const
{
    return _type;
}


bool AnyObject::isType(Type expectedType) const
{
    return (getType() == expectedType);
}


std::ostream &operator<<(std::ostream &out, const AnyObject &object);
