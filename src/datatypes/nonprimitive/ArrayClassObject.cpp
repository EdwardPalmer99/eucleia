/**
 * @file ArrayClassObject.cpp
 * @author Edward Palmer
 * @date 2024-12-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "ArrayClassObject.hpp"
#include "EucleiaUtility.hpp"
#include "ExpressionScope.hpp"
#include "LibraryFunctionObject.hpp"
#include <IntObject.hpp>
#include <iostream>

ModuleClassObject::ModuleClassObject(std::string moduleTypeName_, std::string name_)
    : ClassObject(std::move(moduleTypeName_), std::move(name_))
{
}


BaseObject *ModuleClassObject::evaluate(Scope &scope)
{
    if (active)
    {
        EucleiaError("ModuleClassObject named '%s' of type '%s' is already active.",
                     name.c_str(), typeName.c_str());
    }

    active = true;

    installClassMethods();

    // Add the active array object instance to the scope.
    scope.linkObject(name, this);
    return this;
}


void ModuleClassObject::installClassMethod(std::string methodName, Method method)
{
    LibraryFunctionObject *closureObj = instanceScope.createManagedObject<LibraryFunctionObject>(method);
    instanceScope.linkObject(std::move(methodName), closureObj);
}


ArrayClassObject::ArrayClassObject(std::string name_)
    : ModuleClassObject(ArrayClassObject::className(), std::move(name_))
{
}


void ArrayClassObject::installClassMethods()
{
    // Accessor method.
    auto getClosure = [this](ProgramNode &callArgs, Scope &scope) -> BaseObject *
    {
        assert(callArgs.programNodes.size() == 1); // Just the index.

        // Evaluate as temporary so doesn't persist for long.
        IntObject index = evaluateExpression<IntObject>(callArgs.programNodes.front(), scope);
        std::cout << "objects.size = " << objects.size() << ", index.value = " << index.value << std::endl;
        assert(index.value >= 0 && index.value < objects.size());

        BaseObject *object = objects.at(index.value);

        // Outer scope now has ownership of a copy of object in array.
        return scope.cloneObject(object);
    };

    auto setClosure = [this](ProgramNode &callArgs, Scope &scope) -> BaseObject *
    {
        assert(callArgs.programNodes.size() == 2); // Just the index and new object.

        // Evaluate as temporary so doesn't persist for long.
        IntObject index = evaluateExpression<IntObject>(callArgs.programNodes.front(), scope);
        std::cout << "objects.size = " << objects.size() << ", index.value = " << index.value << std::endl;
        assert(index.value >= 0 && index.value < objects.size());

        BaseObject *object = objects.at(index.value);

        // Do an assignment (will work provided implemented for object types).
        // We add to a temporary expression scope.
        Scope tmpScope(scope);
        BaseObject *someTest = callArgs.programNodes.back()->evaluate(tmpScope);
        *object = *someTest;

        return nullptr;
    };

    auto appendClosure = [this](ProgramNode &callArgs, Scope &scope) -> BaseObject *
    {
        assert(callArgs.programNodes.size() == 1); // Just the object to add.

        // Evaluate in instance scope and then transfer ownership to vector.
        BaseObject *object = callArgs.programNodes.front()->evaluate(this->instanceScope);
        objects.push_back(this->instanceScope.releaseManagedObject(object));

        std::cout << "objects.size = " << objects.size() << std::endl;

        return nullptr;
    };

    auto clearClosure = [this](ProgramNode &callArgs, Scope &scope) -> BaseObject *
    {
        assert(callArgs.programNodes.size() == 0);

        // Vector has ownership of objects (see append method). Therefore, delete.
        for (BaseObject *object : objects)
        {
            delete object;
        }

        objects.clear();
        return nullptr;
    };

    auto lenClosure = [this](ProgramNode &callArgs, Scope &scope) -> BaseObject *
    {
        assert(callArgs.programNodes.size() == 0);

        return scope.createManagedObject<IntObject>(this->objects.size());
    };

    installClassMethod("get", getClosure);
    installClassMethod("set", setClosure);
    installClassMethod("append", appendClosure);
    installClassMethod("clear", clearClosure);
    installClassMethod("len", lenClosure);
}