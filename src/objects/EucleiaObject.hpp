//
//  EucleiaObject.hpp
//  Eucleia
//
//  Created by Edward on 14/01/2024.
//

#ifndef EucleiaObject_hpp
#define EucleiaObject_hpp

#include "EucleiaUtility.hpp"
#include <assert.h>
#include <functional>
#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

struct BaseObject;
class ProgramNode;
class Scope;

// Forwards declaration.
class FunctionNode;
class BaseObject;

class BaseObject
{
public:
    /// Cast to object type.
    template <class TObject>
    const TObject &castObject() const
    {
        return static_cast<const TObject &>(*this);
    }

    template <class TObject>
    TObject &castObject()
    {
        return static_cast<TObject &>(*this);
    }

    /// Check object type.
    template <class TObject>
    bool isObjectType() const
    {
        return typeid(this) == typeid(TObject);
    }

    bool typesMatch(const BaseObject &other) const
    {
        return typeid(this) == typeid(other);
    }

    virtual std::string typeName() const = 0;

protected:
    BaseObject() = default;
    virtual ~BaseObject() = default;
};


class FloatObject : public BaseObject
{
public:
    FloatObject(double value_ = 0.0) : value(value_) {}

    std::string typeName() const override { return "FloatObject"; }

    FloatObject &operator++()
    {
        ++value;
        return *this;
    }

    FloatObject &operator--()
    {
        --value;
        return *this;
    }

    FloatObject operator-() const // Negation.
    {
        return FloatObject(-value);
    }

    // Add addition / subtraction operations.
    FloatObject operator+(const FloatObject &other) const
    {
        return FloatObject(value + other.value);
    }

    FloatObject operator-(const FloatObject &other) const
    {
        return FloatObject(value - other.value);
    }

    FloatObject operator*(const FloatObject &other) const
    {
        return FloatObject(value * other.value);
    }

    FloatObject operator/(const FloatObject &other) const
    {
        return FloatObject(value / other.value);
    }

    bool operator==(const FloatObject &other) const
    {
        return (value == other.value);
    }

    bool operator!=(const FloatObject &other) const
    {
        return (value != other.value);
    }

    bool operator>=(const FloatObject &other) const
    {
        return (value >= other.value);
    }

    bool operator>(const FloatObject &other) const
    {
        return (value > other.value);
    }

    bool operator<=(const FloatObject &other) const
    {
        return (value <= other.value);
    }

    bool operator<(const FloatObject &other) const
    {
        return (value <= other.value);
    }

    double value;
};


class IntObject : public BaseObject
{
public:
    IntObject(long value_ = 0) : value(value_) {}

    std::string typeName() const override { return "IntObject"; }

    FloatObject castToFloat() const
    {
        return FloatObject((double)value);
    }

    IntObject &operator++()
    {
        ++value;
        return *this;
    }

    IntObject &operator--()
    {
        --value;
        return *this;
    }

    IntObject operator-() const // Negation.
    {
        return IntObject(-value);
    }

    IntObject operator!() const // Not.
    {
        bool state = (value > 0);
        return IntObject(!state);
    }

    IntObject operator+(const IntObject &other) const
    {
        return IntObject(value + other.value);
    }

    IntObject operator-(const IntObject &other) const
    {
        return IntObject(value - other.value);
    }

    IntObject operator*(const IntObject &other) const
    {
        return IntObject(value * other.value);
    }

    IntObject operator/(const IntObject &other) const
    {
        return IntObject(value / other.value);
    }

    IntObject operator==(const IntObject &other) const
    {
        return IntObject(value == other.value);
    }

    IntObject operator!=(const IntObject &other) const
    {
        return IntObject(value != other.value);
    }

    IntObject operator>=(const IntObject &other) const
    {
        return IntObject(value >= other.value);
    }

    IntObject operator>(const IntObject &other) const
    {
        return IntObject(value > other.value);
    }

    IntObject operator<=(const IntObject &other) const
    {
        return IntObject(value <= other.value);
    }

    IntObject operator<(const IntObject &other) const
    {
        return IntObject(value <= other.value);
    }

    IntObject operator%(const IntObject &other) const
    {
        // TODO: - add safety check.
        return IntObject(value % other.value);
    }

    IntObject operator&&(const IntObject &other) const
    {
        return IntObject(value && other.value);
    }

    IntObject operator||(const IntObject &other) const
    {
        return IntObject(value || other.value);
    }

    long value;
};


using BoolObject = IntObject;


class StringObject : public BaseObject
{
public:
    StringObject(std::string value_ = "") : value(std::move(value_)) {}

    std::string typeName() const override { return "StringObject"; }


    // TODO: - add string operations here such as operations string + string

    std::string value;
};


inline std::ostream &operator<<(std::ostream &out, const BaseObject &object)
{
    if (object.isObjectType<IntObject>())
        return (out << object.castObject<IntObject>().value);
    else if (object.isObjectType<FloatObject>())
        return (out << object.castObject<FloatObject>().value);
    else if (object.isObjectType<StringObject>())
        return (out << object.castObject<StringObject>().value);
    else
        return out; // Don't print anything.
}


// struct ArrayObject : public BaseObject
// {
//     ArrayObject() = delete;
//     ArrayObject(const std::vector<std::shared_ptr<BaseObject>> values)
//         : arrayValues{std::move(values)}
//     {
//     }

//     inline ObjectType type() const override
//     {
//         return ObjectType::Array;
//     }

//     const std::vector<std::shared_ptr<BaseObject>> arrayValues;
// };

// /// FunctionObject contains a pointer to the original function definition which
// /// allows us to call its evaluate() method and perform type-checking of the
// /// supplied function arguments with the expected arguments.
// struct FunctionObject : public BaseObject
// {
//     FunctionObject() = delete;
//     FunctionObject(std::shared_ptr<FunctionNode> function);

//     inline ObjectType type() const override
//     {
//         return ObjectType::Function;
//     }

//     std::shared_ptr<FunctionNode> functionValue{nullptr};
// };

// /// Library function allows us to define lambdas which wrap around existing stdlib
// /// functions. These can then be added to a global scope after seeing "import <...>"
// /// with angled-brackets.
// struct LibraryFunctionObject : public BaseObject
// {
//     using LibraryFunction = std::function<std::shared_ptr<BaseObject>(ProgramNode &, Scope &)>;

//     LibraryFunctionObject() = delete;
//     LibraryFunctionObject(LibraryFunction function)
//         : evaluate{std::move(function)}
//     {
//     }

//     inline ObjectType type() const override
//     {
//         return ObjectType::LibraryFunction;
//     }

//     LibraryFunction evaluate{nullptr};
// };

// #pragma mark -


// inline std::ostream &operator<<(std::ostream &out, const ArrayObject &object)
// {
//     out << "[";
//     for (const auto &value : object.arrayValues)
//     {
//         out << value; // BUG

//         if (value != object.arrayValues.back())
//         {
//             out << ", ";
//         }
//     }
//     out << "]";

//     return out;
// }


// #pragma mark -

// inline const IntObject &castToInt(const BaseObject &object)
// {
//     assert(object.type() == BaseObject::ObjectType::Int);

//     auto ptr = static_cast<const IntObject *>(&object);
//     return *ptr;
// }

// inline const FloatObject &castToFloat(const BaseObject &object)
// {
//     assert(object.type() == BaseObject::ObjectType::Float);

//     auto ptr = static_cast<const FloatObject *>(&object);
//     return *ptr;
// }

// inline const BoolObject &castToBool(const BaseObject &object)
// {
//     assert(object.type() == BaseObject::ObjectType::Bool);

//     auto ptr = static_cast<const BoolObject *>(&object);
//     return *ptr;
// }

#endif /* EucleiaObject_hpp */
