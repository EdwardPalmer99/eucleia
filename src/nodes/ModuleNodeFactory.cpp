//
//  ModuleNodeFactory.cpp
//  Eucleia
//
//  Created by Edward on 11/02/2024.
//

#include "ModuleNodeFactory.hpp"
#include "BaseNode.hpp"
#include "Exceptions.hpp"
#include "Logger.hpp"
#include "NodeFactory.hpp"
#include "ObjectFactory.hpp"
#include "Stringify.hpp"
#include <cassert>
#include <cmath>
#include <iostream>


namespace NodeFactory
{

AnyNode::Ptr createDefinedModuleNode(std::string moduleName)
{
    if (moduleName == "io")
        return createIOModuleNode();
    else if (moduleName == "math")
        return createMathModuleNode();
    else if (moduleName == "stdarray")
        return createArrayModuleNode();
    else if (moduleName == "test")
        return createTestModuleNode();
    else
        ThrowException("unknown module node '" + moduleName + "'");
}


AnyNode::Ptr createIOModuleNode()
{
    auto doPrint = std::pair("print", [](BaseNodePtrVector &callArgs, Scope &scope)
    {
        for (const auto &callArg : callArgs)
        {
            auto evaluatedNode = callArg->evaluate(scope);

            std::cout << *evaluatedNode;

            if (callArg != callArgs.back())
            {
                std::cout << " ";
            }
        }

        std::cout << std::endl;
        return nullptr;
    });

    return NodeFactory::createModuleNode("io", {doPrint});
}


AnyNode::Ptr createMathModuleNode()
{
    auto doSqrt = std::pair("sqrt", [](BaseNodePtrVector &callArgs, Scope &scope)
    {
        assert(callArgs.size() == 1);

        double first = callArgs.front()->evaluate(scope)->getValue<double>();
        return ObjectFactory::allocate(sqrt(first));
    });

    auto doPow = std::pair("pow", [](BaseNodePtrVector &callArgs, Scope &scope)
    {
        assert(callArgs.size() == 2);

        /* TODO: - bug if pass in an integer rather than a floatObject */
        auto firstObject = callArgs.front()->evaluate(scope);
        auto secondObject = callArgs.front()->evaluate(scope);

        if (firstObject->isType(AnyObject::Int) && secondObject->isType(AnyObject::Int)) /* Implicit casts */
        {
            return ObjectFactory::allocate(pow(firstObject->getValue<long>(), secondObject->getValue<long>()));
        }
        else /* Assume both are doubles */
        {
            return ObjectFactory::allocate(pow(firstObject->getValue<double>(), secondObject->getValue<double>()));
        }
    });

    return NodeFactory::createModuleNode("math", {doSqrt, doPow});
}


AnyNode::Ptr createArrayModuleNode()
{
    auto doClear = std::pair("clear", [](BaseNodePtrVector &callArgs, Scope &scope)
    {
        assert(callArgs.size() == 1);

        auto theObject = callArgs.front()->evaluate(scope); /* Careful if using a reference! Need to not hold reference to garbage! */

        auto &arrayObject = theObject->getValue<AnyObject::Vector>();
        arrayObject.clear();
        return nullptr;
    });

    auto doLength = std::pair("length", [](BaseNodePtrVector &callArgs, Scope &scope)
    {
        assert(callArgs.size() == 1);

        auto theObject = callArgs.front()->evaluate(scope);

        auto &arrayObject = theObject->getValue<AnyObject::Vector>();

        return ObjectFactory::allocate((double)arrayObject.size());
    });

    auto doAppend = std::pair("append", [](BaseNodePtrVector &callArgs, Scope &scope)
    {
        assert(callArgs.size() == 2);

        auto theObject = callArgs.front()->evaluate(scope);

        auto &arrayObject = theObject->getValue<AnyObject::Vector>();
        auto someObject = callArgs.back()->evaluate(scope);

        arrayObject.push_back(someObject->clone()); // NB: must clone!
        return nullptr;
    });

    return NodeFactory::createModuleNode("array", {doClear, doLength, doAppend});
}


AnyNode::Ptr createTestModuleNode()
{
    auto doTest = std::pair("TEST", [](BaseNodePtrVector &callArgs, Scope &scope)
    {
        using namespace eucleia;

        const char *ClearColor{"\033[0m"};
        const char *FailColor{"\033[91m"};
        const char *PassColor{"\033[92m"};

        assert(callArgs.size() == 2);

        bool result = callArgs.front()->evaluate(scope)->getValue<bool>();
        std::string description = callArgs.back()->evaluate(scope)->getValue<std::string>(); /* Be very careful -> copy otherwise referencing garbage! */

        // Print pass or fail depending on the test case.
        const char *statusString = result ? "PASSED" : "FAILED";
        const char *statusColor = result ? PassColor : FailColor;

        std::cout << stringify("%-50s %s%s%s", description.c_str(), statusColor, statusString, ClearColor) << std::endl;
        return nullptr;
    });

    return NodeFactory::createModuleNode("test", {doTest});
}


} // namespace NodeFactory