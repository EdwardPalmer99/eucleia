/**
 * @file AddVariableNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "AnyObject.hpp"
#include "LookupVariableNode.hpp"
#include "Scope.hpp"


class AddVariableNode : public LookupVariableNode
{
public:
    using Ptr = std::shared_ptr<AddVariableNode>;

    AddVariableNode(std::string name, AnyObject::Type type);

    // Creates a new empty variable of a given type to the scope (i.e. int a;).
    AnyObject evaluate(Scope &scope) override;
    AnyObject::Ref evaluateRef(Scope &scope) override;

    //  Type checking for variable assignment.
    bool passesAssignmentTypeCheck(const AnyObject &assignObject) const;

    AnyObject::Type variableType() const { return _variableType; }

protected:
    const AnyObject::Type _variableType;
};


/**
 * Construct a reference to an existing variable declared in the scope or a parent scope. This is similar to C++ and
 * avoids unnecessary copies.
 */
class AddReferenceVariableNode : public AddVariableNode
{
public:
    /**
     * @param referenceName_ Name of the reference.
     * @param boundName_ Name of the variable to be bound to the reference.
     * @param boundType_ Type of the bound variable. To be checked when evaluate() called.
     */
    AddReferenceVariableNode(std::string referenceName_, std::string boundName_, AnyObject::Type boundType_);

    /**
     * @param scope
     * @return Pointer to the object in the scope now bound to the variable name and
     * the reference name.
     */
    AnyObject::Ref evaluateRef(Scope &scope) override;

    AnyObject evaluate(Scope &scope);

protected:
    const std::string referenceName;
};