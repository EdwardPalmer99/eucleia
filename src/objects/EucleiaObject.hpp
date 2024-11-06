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
#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

class ProgramNode;
class Scope;

// Forwards declaration.
class FunctionNode;
class FloatObject;

/**
 * BaseObject. All objects are derived from this class.
 */
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
        return typeid(*this) == typeid(TObject);
    }

    bool typesMatch(const BaseObject &other) const
    {
        return typeid(*this) == typeid(other);
    }

    virtual std::string typeName() const = 0;

protected:
    BaseObject() = default;
    virtual ~BaseObject() = default;
};

/**
 * @brief Long integer type.
 */
class IntObject : public BaseObject
{
public:
    IntObject(long value_ = 0) : value(value_) {}

    std::string typeName() const override { return "IntObject"; }

    FloatObject castToFloat() const;

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
        assert(other.value > 0);
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

/**
 * Class for double type.
 */
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

    IntObject operator==(const FloatObject &other) const
    {
        return IntObject(value == other.value);
    }

    IntObject operator!=(const FloatObject &other) const
    {
        return IntObject(value != other.value);
    }

    IntObject operator>=(const FloatObject &other) const
    {
        return IntObject(value >= other.value);
    }

    IntObject operator>(const FloatObject &other) const
    {
        return IntObject(value > other.value);
    }

    IntObject operator<=(const FloatObject &other) const
    {
        return IntObject(value <= other.value);
    }

    IntObject operator<(const FloatObject &other) const
    {
        return IntObject(value <= other.value);
    }

    double value;
};


class StringObject : public BaseObject
{
public:
    StringObject(std::string value_ = "") : value(std::move(value_)) {}

    std::string typeName() const override { return "StringObject"; }


    StringObject operator+(const StringObject &other) const
    {
        return StringObject(value + other.value);
    }

    StringObject &operator+=(const StringObject &other)
    {
        value += other.value;
        return *this;
    }


    IntObject operator==(const StringObject &other) const
    {
        return IntObject((value == other.value));
    }


    IntObject operator!=(const StringObject &other) const
    {
        return IntObject((value != other.value));
    }

    std::string value;
};


struct ArrayObject : public BaseObject
{
public:
    ArrayObject() = default;
    ArrayObject(std::vector<std::shared_ptr<BaseObject>> values_) : values(std::move(values_)) {}

    std::string typeName() const override { return "ArrayObject"; }

    // TODO: - eventually just store references to BaseObject & or pointers and return reference.
    std::shared_ptr<BaseObject> operator[](std::size_t index) const
    {
        assert(index < values.size());
        return (values.at(index));
    }

    std::vector<std::shared_ptr<BaseObject>> values;
};


/// FunctionObject contains a pointer to the original function definition which
/// allows us to call its evaluate() method and perform type-checking of the
/// supplied function arguments with the expected arguments.
class FunctionObject : public BaseObject
{
public:
    FunctionObject(std::shared_ptr<FunctionNode> function_) : functionValue(std::move(function_)) {}

    std::string typeName() const override { return "FunctionObject"; }

    std::shared_ptr<FunctionNode> functionValue{nullptr};
};


/// Library function allows us to define lambdas which wrap around existing stdlib
/// functions. These can then be added to a global scope after seeing "import <...>"
/// with angled-brackets.
struct LibraryFunctionObject : public BaseObject
{
public:
    using LibraryFunction = std::function<std::shared_ptr<BaseObject>(ProgramNode &, Scope &)>;

    LibraryFunctionObject(LibraryFunction function_) : evaluate(std::move(function_)) {}

    std::string typeName() const override { return "LibraryFunctionObject"; }

    LibraryFunction evaluate;
};


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
    else
        return out; // Don't print anything.
}

#endif /* EucleiaObject_hpp */
