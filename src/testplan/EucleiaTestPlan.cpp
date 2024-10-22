#include "EucleiaTestPlan.hpp"

TestSuite integerTestSuite();
TestSuite stringTestSuite();
TestSuite loopTestSuite();
TestSuite assertTestSuite();


void TestPlan::addTestSuite(TestSuite testSuite)
{
    testSuites.push_back(std::move(testSuite));
}


void TestPlan::execute()
{
    for (auto & testSuite : testSuites)
    {
        testSuite.execute();
    }
}


TestSuite integerTestSuite()
{
    TestSuite suite("integers");

    suite.addTestCase("intAddition", "import <io>\nprint(3 + 2);", "5\n");
    suite.addTestCase("intSubtraction", "import <io>\nprint(3 + 2);", "5\n");
    suite.addTestCase("intMultiply", "import <io>\nprint(3 * 2);", "6\n");
    //suite.addTestCase("intDivision", "import <io>\n; a = 2.1 / 3;print(4 / 2);", "1\n");

    return suite;
}


TestSuite stringTestSuite()
{
    TestSuite suite("strings");

    suite.addTestCase("stringAddition", "import <io>\nprint(\"hello, world!\");", "hello, world!\n");
    
    return suite;
}


TestSuite loopTestSuite()
{
    TestSuite suite("loops");

    suite.addTestCase(
        "forLoop", 
        "import <io>\nfor (int i = 0; i < 2; ++i)\n{\n\tprint(i);\n}", 
        "0\n1\n"
    );

    return suite;
}


TestSuite assertTestSuite()
{
    TestSuite suite("assertions");

    suite.addTestCase(
        "assertString", 
        "import <test>\nASSERT_STR_EQUAL(\"hello\", \"hello\");", ""
    );

    return suite;
}


void executeTestPlan()
{
    TestPlan plan;

    plan.addTestSuite(stringTestSuite());
    plan.addTestSuite(integerTestSuite());
    plan.addTestSuite(loopTestSuite());
    plan.addTestSuite(assertTestSuite());

    plan.execute();
}