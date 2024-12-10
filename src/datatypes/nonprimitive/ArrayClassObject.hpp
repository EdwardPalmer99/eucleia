/**
 * @file ArrayClassObject.hpp
 * @author Edward Palmer
 * @date 2024-12-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseNode.hpp"
#include "BaseObject.hpp"
#include "ClassAccessNode.hpp"
#include "ClassObject.hpp"
#include <functional>
#include <string>
#include <vector>

// TODO : -should require < array> header.
/**
 * Module class consisting of a series of methods.
 */
class ModuleClassObject : public ClassObject
{
public:
    ModuleClassObject(std::string moduleTypeName_, std::string name_);

    /**
     * Once called, install defined methods into class scope.
     */
    BaseObject *evaluate(Scope &scope) final;

protected:
    using Method = std::function<BaseObject *(ProgramNode &, Scope &)>;

    /**
     * Implement in derived classes to install class methods.
     */
    virtual void installClassMethods() = 0;

    /**
     * Install a method in the instance scope.
     */
    void installClassMethod(std::string methodName, Method method);
};


/**
 * Array class.
 */
class ArrayClassObject : public ModuleClassObject
{
public:
    static std::string className() { return "array"; }

    ArrayClassObject(std::string name_);

    friend std::ostream &operator<<(std::ostream &os, const BaseObject &);

protected:
    /**
     * Installs array methods.
     */
    void installClassMethods() override;

private:
    /**
     * Array of pointers. One for each object. These are owned by the instanceScope.
     * so we do not explicitly need to delete these from the vector.
     */
    std::vector<BaseObject *> objects;
};
