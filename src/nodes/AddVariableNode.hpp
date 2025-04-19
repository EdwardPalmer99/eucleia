/**
 * @file AddVariableNode.hpp
 * @author Edward Palmer
 * @date 2024-11-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseObject.hpp"
#include "LookupVariableNode.hpp"
#include "ObjectTypes.hpp"
#include "Scope.hpp"

class FileParser;


class AddVariableNode : public LookupVariableNode
{
public:
    AddVariableNode(std::string name_, ObjectType type_)
        : LookupVariableNode(std::move(name_)),
          type(type_) {}

    // Creates a new empty variable of a given type to the scope (i.e. int a;).
    BaseObject *evaluate(Scope &scope) override;

    /**
     * Parses a reference or variable definition
     *
     * Variable definition:
     * int/float/string/bool/array [VARIABLE_NAME]
     */
    static AddVariableNode *parse(FileParser &parser);

    std::string description() const;

    //  Type checking for variable assignment.
    bool passesAssignmentTypeCheck(const BaseObject &assignObject) const;

    const ObjectType type;
};


/**
 * Construct a reference to an existing variable declared in the scope or a parent
 * scope. This is similar to C++ and avoids unnecessary copies.
 */
class AddReferenceVariableNode : public AddVariableNode
{
public:
    /**
     * @param referenceName_ Name of the reference.
     * @param boundName_ Name of the variable to be bound to the reference.
     * @param boundType_ Type of the bound variable. To be checked when evaluate() called.
     */
    AddReferenceVariableNode(std::string referenceName_, std::string boundName_, ObjectType boundType_);

    /**
     * @param scope
     * @return Pointer to the object in the scope now bound to the variable name and
     * the reference name.
     */
    BaseObject *evaluate(Scope &scope) override;

    /**
     * Bind a reference to an already declared variable in this scope or a parent
     * scope. We do not allow any unbound references and once bound, references
     * cannot be unbound. By default, we pass by value to functions, but the use
     * of a reference as in other languages such as C++ avoids copying.
     *
     * Reference definition:
     * VARIABLE_TO_BIND_TO_TYPE & REFERENCE_NAME = VARIABLE_TO_BIND_TO;
     */
    static AddReferenceVariableNode *parse(FileParser &parser, ObjectType boundVariableType);

protected:
    const std::string referenceName;
};