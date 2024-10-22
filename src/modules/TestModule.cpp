/**
 * @file TestModule.cpp
 * @author Edward Palmer
 * @date 2024-10-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

 #include "TestModule.hpp"

 void TestModule::defineFunctions()
 {
    defineFunction("ASSERT", [=](ProgramNode  & callArgs, Scope & scope) -> std::shared_ptr<BaseObject>
    {
        auto argValues = evaluateArgs(callArgs, scope);
        assert(argValues.size() == 1);

        auto result = argValues.at(0)->boolCast();
        assert(result);

        return nullptr;
    });

    defineFunction("ASSERT_STR_EQUAL", [=](ProgramNode  & callArgs, Scope & scope) -> std::shared_ptr<BaseObject>
    {
        auto argValues = evaluateArgs(callArgs, scope);
        assert(argValues.size() == 2);

        StringObject & str1 = static_cast<StringObject &>(*argValues.at(0));
        StringObject & str2 = static_cast<StringObject &>(*argValues.at(1));

        assert(str1.stringValue == str2.stringValue);

        return nullptr;
    });

    defineFunction("abort", [=](ProgramNode  & callArgs, Scope & scope) -> std::shared_ptr<BaseObject>
    {
        auto argValues = evaluateArgs(callArgs, scope);
        assert(argValues.size() == 0);

        abort();
        return nullptr;
    });

    defineFunction("exit", [=](ProgramNode  & callArgs, Scope & scope) -> std::shared_ptr<BaseObject>
    {
        auto argValues = evaluateArgs(callArgs, scope);
        assert(argValues.size() == 1);

        auto exitCode = argValues.at(0)->intCast();
        exit(exitCode);

        return nullptr;
    });
 }