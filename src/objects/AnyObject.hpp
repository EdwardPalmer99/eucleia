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
#include "Scope.hpp"
#include <functional>
#include <memory>
#include <string>
#include <variant>
#include <vector>

class AnyObject; /* Forward declaration */

class ModuleFunctor
{
public:
    using ModuleFunction = std::function<std::shared_ptr<AnyObject>(BaseNodePtrVector &, Scope &)>;

    ModuleFunctor(ModuleFunction &&function) : _function(std::move(function)) {}

    [[nodiscard]] inline std::shared_ptr<AnyObject> operator()(BaseNodePtrVector &args, Scope &scope)
    {
        return !_function ? nullptr : _function(args, scope);
    }

private:
    ModuleFunction _function{nullptr};
};


class AnyObject
{
public:
    using Ptr = std::shared_ptr<AnyObject>;
    using Vector = std::vector<AnyObject::Ptr>;

    enum Type
    {
        None = (-1),
        Int,
        Float,
        String,
        Array,
        ModuleFunction
    };

    constexpr explicit AnyObject(long value) : _value(value) {} /* NB: require explicit to avoid implicit casting */
    constexpr explicit AnyObject(double value) : _value(value) {}
    constexpr explicit AnyObject(AnyObject::Vector &&value) : _value(std::move(value)) {}
    constexpr explicit AnyObject(ModuleFunctor &&value) : _value(std::move(value)) {}

    template <typename TValue>
    [[nodiscard]] inline TValue &getValue();

    template <typename TValue>
    [[nodiscard]] inline const TValue &getValue() const;

    [[nodiscard]] inline Type getType() const;

    /* TODO: - think about copy constructors/assignment here. Will need to implement based on types */

protected:
    AnyObject() = default;          /* Prevent direct initialization */
    virtual ~AnyObject() = default; /* In case we subclass */

private:
    std::variant<long, double, std::string, Vector, ModuleFunctor> _value;
};


template <typename TValue>
TValue &AnyObject::getValue()
{
    return std::get<TValue &>(_value);
}


template <typename TValue>
const TValue &AnyObject::getValue() const
{
    return std::get<const TValue &>(_value);
}


AnyObject::Type AnyObject::getType() const
{
    return Type(_value.index());
}