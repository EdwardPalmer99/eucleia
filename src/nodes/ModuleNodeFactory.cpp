//
//  ModuleNodeFactory.cpp
//  Eucleia
//
//  Created by Edward on 11/02/2024.
//

#include "ModuleNodeFactory.hpp"
#include "ArrayObject.hpp"
#include "BaseNode.hpp"
#include "FloatObject.hpp"
#include "IntObject.hpp"
#include "Logger.hpp"
#include "NodeFactory.hpp"
#include "Objects.hpp"
#include "StringObject.hpp"
#include "Stringify.hpp"
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
    else if (moduleName == "array")
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

        auto first = callArgs.front()->evaluate<FloatObject>(scope);
        return ObjectFactory::allocate<FloatObject>(sqrt(first->value()));
    });

    auto doPow = std::pair("pow", [](BaseNodePtrVector &callArgs, Scope &scope)
    {
        assert(callArgs.size() == 2);

        auto first = callArgs.front()->evaluate<FloatObject>(scope);
        auto second = callArgs.back()->evaluate<FloatObject>(scope);

        return ObjectFactory::allocate<FloatObject>(pow(first->value(), second->value()));
    });

    return NodeFactory::createModuleNode("math", {doSqrt, doPow});
}


AnyNode::Ptr createArrayModuleNode()
{
    auto doClear = std::pair("clear", [](BaseNodePtrVector &callArgs, Scope &scope)
    {
        assert(callArgs.size() == 1);

        auto arrayObject = callArgs.front()->evaluate<ArrayObject>(scope);
        arrayObject->value().clear();
        return nullptr;
    });

    auto doLength = std::pair("length", [](BaseNodePtrVector &callArgs, Scope &scope)
    {
        assert(callArgs.size() == 1);

        auto arrayObject = callArgs.front()->evaluate<ArrayObject>(scope);

        return ObjectFactory::allocate<IntObject>(arrayObject->value().size());
    });

    auto doAppend = std::pair("append", [](BaseNodePtrVector &callArgs, Scope &scope)
    {
        assert(callArgs.size() == 2);

        auto arrayObject = callArgs.front()->evaluate<ArrayObject>(scope);
        auto someObject = callArgs.back()->evaluate(scope);

        arrayObject->value().push_back(someObject->clone()); // NB: must clone!
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

        auto result = callArgs.front()->evaluate<BoolObject>(scope);
        auto description = callArgs.back()->evaluate<StringObject>(scope);

        // Print pass or fail depending on the test case.
        const char *statusString = result->value() ? "PASSED" : "FAILED";
        const char *statusColor = result->value() ? PassColor : FailColor;

        std::cout << stringify("%-50s %s%s%s", description->value().c_str(), statusColor, statusString, ClearColor) << std::endl;
        return nullptr;
    });

    return NodeFactory::createModuleNode("test", {doTest});
}


} // namespace NodeFactory