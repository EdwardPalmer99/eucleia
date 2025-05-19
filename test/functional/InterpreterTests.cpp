/**
 * @file InterpreterTests.cpp
 * @author Edward Palmer
 * @date 2025-04-16
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "EucleiaInterpreter.hpp"
#include "Scope.hpp"
#include "test/utility/Utility.hpp"
#include <cstdlib>
#include <gtest/gtest.h>


std::string testDataPath(std::string fileName);


TEST(InterpreterTestSuite, ArrayTests)
{
    Interpreter::evaluateFile(testDataPath("ArrayTests.ek"));
}


TEST(InterpreterTestSuite, BoolTests)
{
    Interpreter::evaluateFile(testDataPath("BoolTests.ek"));
}


TEST(InterpreterTestSuite, ClassTests)
{
    Interpreter::evaluateFile(testDataPath("ClassTests.ek"));
}


TEST(InterpreterTestSuite, InheritanceTests)
{
    Interpreter::evaluateFile(testDataPath("InheritanceTests.ek"));
}


TEST(InterpreterTestSuite, IntTests)
{
    Interpreter::evaluateFile(testDataPath("IntTests.ek"));
}


TEST(InterpreterTestSuite, LoopTests)
{
    Interpreter::evaluateFile(testDataPath("LoopTests.ek"));
}


TEST(InterpreterTestSuite, MiscTests)
{
    Interpreter::evaluateFile(testDataPath("MiscTests.ek"));
}


TEST(InterpreterTestSuite, ReferenceTests)
{
    Interpreter::evaluateFile(testDataPath("ReferenceTests.ek"));
}


TEST(InterpreterTestSuite, ScopeTests)
{
    Interpreter::evaluateFile(testDataPath("ScopeTests.ek"));
}


TEST(InterpreterTestSuite, StringTests)
{
    Interpreter::evaluateFile(testDataPath("StringTests.ek"));
}


TEST(InterpreterTestSuite, StructTests)
{
    Interpreter::evaluateFile(testDataPath("StructTests.ek"));
}


TEST(InterpreterTestSuite, CastTests)
{
    Interpreter::evaluateFile(testDataPath("CastTests.ek"));
}


std::string testDataPath(std::string fileName)
{
    return getTestDirPath() + "functional/data/" + fileName;
}