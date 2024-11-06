/**
 * @file TestModule.cpp
 * @author Edward Palmer
 * @date 2024-10-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "TestModule.hpp"

static const char *kClearColor = "\033[0m";
static const char *kFailColor = "\033[91m";
static const char *kOkGreenColor = "\033[92m";


void TestModule::defineFunctions()
{
    defineFunction("TEST", [=](ProgramNode &callArgs, Scope &scope) -> std::shared_ptr<BaseObject>
                   {
        auto argValues = evaluateArgs(callArgs, scope);
        assert(argValues.size() == 2);

        auto result = argValues.at(0)->castObject<BoolObject>();
        auto description = argValues.at(1)->castObject<StringObject>();

        // Print pass or fail depending on the test case.
        const char *statusString = result.value ? "PASSED" : "FAILED";
        const char *statusColor  = result.value ? kOkGreenColor : kFailColor;

        fflush(stdout);
        fprintf(stdout, "%-50s %s%s%s\n", description.value.c_str(), statusColor, statusString,  kClearColor);
        if (!result.value) exit(EXIT_FAILURE);
        return nullptr; });

    defineFunction("ASSERT", [=](ProgramNode &callArgs, Scope &scope) -> std::shared_ptr<BaseObject>
                   {
        auto argValues = evaluateArgs(callArgs, scope);
        assert(argValues.size() == 1);

        auto result = argValues.at(0)->castObject<BoolObject>();
        assert(result.value);
        return nullptr; });

    defineFunction("abort", [=](ProgramNode &callArgs, Scope &scope) -> std::shared_ptr<BaseObject>
                   {
        auto argValues = evaluateArgs(callArgs, scope);
        assert(argValues.size() == 0);

        abort();
        return nullptr; });

    defineFunction("exit", [=](ProgramNode &callArgs, Scope &scope) -> std::shared_ptr<BaseObject>
                   {
        auto argValues = evaluateArgs(callArgs, scope);
        assert(argValues.size() == 1);

        auto exitCode = argValues.at(0)->castObject<IntObject>();
        exit(exitCode.value);

        return nullptr; });
}