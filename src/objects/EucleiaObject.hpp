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

struct BaseObject
{
    BaseObject() = default;
    virtual ~BaseObject()
    {
    }

    enum class ObjectType
    {
        None,
        Bool,
        Int,
        Float,
        String,
        Array,
        Function, // Function definition.
        LibraryFunction
    };

    std::string description() const;

    inline virtual ObjectType type() const
    {
        return ObjectType::None;
    }

    /// Increment value of object if defined.
    inline virtual void incrementValue()
    {
        printWarpError("Increment operator not defined for object of type '%s'.\n", description().c_str());
    }

    inline virtual void decrementValue()
    {
        printWarpError("Decrement operator not defined for object of type '%s'.\n", description().c_str());
    }

    virtual std::shared_ptr<BaseObject> negateValue()
    {
        printWarpError("Negation operator not defined for object of type '%s'.\n", description().c_str());
    }

    double floatCast() const;
    long intCast() const;
    bool boolCast() const;
};

struct IntObject : public BaseObject
{
    IntObject() = delete;
    IntObject(const long _value = 0)
        : value{_value}
    {
    }

    inline ObjectType type() const override
    {
        return ObjectType::Int;
    }

    inline void incrementValue() override
    {
        ++value;
    }
    inline void decrementValue() override
    {
        --value;
    }

    std::shared_ptr<BaseObject> negateValue() override
    {
        return std::make_shared<IntObject>(-value);
    }

    long positiveValue() const;
    long positiveOrZeroValue() const;
    long nonzeroValue() const;

    long value{0}; // Remove const qualification so we can modify value.
};

struct FloatObject : public BaseObject
{
    FloatObject() = delete;
    FloatObject(const double _value = 0.0)
        : value{_value}
    {
    }

    inline void incrementValue() override
    {
        ++value;
    }
    inline void decrementValue() override
    {
        --value;
    }

    std::shared_ptr<BaseObject> negateValue() override
    {
        return std::make_shared<FloatObject>(-value);
    }

    inline ObjectType type() const override
    {
        return ObjectType::Float;
    }

    double value;
};

struct BoolObject : public BaseObject
{
    BoolObject() = delete;
    BoolObject(const bool _value = false)
        : value{_value}
    {
    }

    inline ObjectType type() const override
    {
        return ObjectType::Bool;
    }

    const bool value;
};

struct StringObject : public BaseObject
{
    StringObject() = delete;
    StringObject(const std::string &value = "")
        : stringValue{value}
    {
    }

    inline ObjectType type() const override
    {
        return ObjectType::String;
    }

    const std::string stringValue;
};

struct ArrayObject : public BaseObject
{
    ArrayObject() = delete;
    ArrayObject(const std::vector<std::shared_ptr<BaseObject>> values)
        : arrayValues{std::move(values)}
    {
    }

    inline ObjectType type() const override
    {
        return ObjectType::Array;
    }

    const std::vector<std::shared_ptr<BaseObject>> arrayValues;
};

/// FunctionObject contains a pointer to the original function definition which
/// allows us to call its evaluate() method and perform type-checking of the
/// supplied function arguments with the expected arguments.
struct FunctionObject : public BaseObject
{
    FunctionObject() = delete;
    FunctionObject(std::shared_ptr<FunctionNode> function);

    inline ObjectType type() const override
    {
        return ObjectType::Function;
    }

    std::shared_ptr<FunctionNode> functionValue{nullptr};
};

/// Library function allows us to define lambdas which wrap around existing stdlib
/// functions. These can then be added to a global scope after seeing "import <...>"
/// with angled-brackets.
struct LibraryFunctionObject : public BaseObject
{
    using LibraryFunction = std::function<std::shared_ptr<BaseObject>(ProgramNode &, Scope &)>;

    LibraryFunctionObject() = delete;
    LibraryFunctionObject(LibraryFunction function)
        : evaluate{std::move(function)}
    {
    }

    inline ObjectType type() const override
    {
        return ObjectType::LibraryFunction;
    }

    LibraryFunction evaluate{nullptr};
};

#pragma mark -

inline std::ostream &operator<<(std::ostream &out, const IntObject &object)
{
    return (out << object.value);
}

inline std::ostream &operator<<(std::ostream &out, const FloatObject &object)
{
    return (out << object.value);
}

inline std::ostream &operator<<(std::ostream &out, const BoolObject &object)
{
    return (out << (object.value ? "true" : "false"));
}

inline std::ostream &operator<<(std::ostream &out, const StringObject &object)
{
    return (out << object.stringValue);
}

inline std::ostream &operator<<(std::ostream &out, const ArrayObject &object)
{
    out << "[";
    for (const auto &value : object.arrayValues)
    {
        out << value; // BUG

        if (value != object.arrayValues.back())
        {
            out << ", ";
        }
    }
    out << "]";

    return out;
}

inline std::ostream &operator<<(std::ostream &out, const BaseObject &object)
{
    switch (object.type())
    {
        case BaseObject::ObjectType::Int:
            return (out << *static_cast<const IntObject *>(&object));
        case BaseObject::ObjectType::Float:
            return (out << *static_cast<const FloatObject *>(&object));
        case BaseObject::ObjectType::Bool:
            return (out << *static_cast<const BoolObject *>(&object));
        case BaseObject::ObjectType::String:
            return (out << *static_cast<const StringObject *>(&object));
        case BaseObject::ObjectType::Array:
            return (out << *static_cast<const ArrayObject *>(&object));
        default:
            return out; // Don't add anything.
    }
}

#pragma mark -

inline const IntObject &castToInt(const BaseObject &object)
{
    assert(object.type() == BaseObject::ObjectType::Int);

    auto ptr = static_cast<const IntObject *>(&object);
    return *ptr;
}

inline const FloatObject &castToFloat(const BaseObject &object)
{
    assert(object.type() == BaseObject::ObjectType::Float);

    auto ptr = static_cast<const FloatObject *>(&object);
    return *ptr;
}

inline const BoolObject &castToBool(const BaseObject &object)
{
    assert(object.type() == BaseObject::ObjectType::Bool);

    auto ptr = static_cast<const BoolObject *>(&object);
    return *ptr;
}

#endif /* EucleiaObject_hpp */
