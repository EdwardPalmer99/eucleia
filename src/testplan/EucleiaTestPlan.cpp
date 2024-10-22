#include "EucleiaTestPlan.hpp"

TestSuite integerTestSuite();
TestSuite stringTestSuite();


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

    suite.addTestCase("intAddition", "import <test>\nASSERT((3 + 2) == 5);");
    suite.addTestCase("intSubtraction", "import <test>\nASSERT((3 + 2) == 5);");
    suite.addTestCase("intMultiply", "import <test>\nASSERT((3 * 2) == 6);");
    //suite.addTestCase("intDivision", "import <test>\n; a = 2.1 / 3;print(4 / 2);", "1\n");

    return suite;
}


TestSuite stringTestSuite()
{
    TestSuite suite("strings");

    suite.addTestCase("stringAddition", "import <test>\nASSERT_STR_EQUAL(\"hello, world!\", \"hello, world!\");");
    
    return suite;
}


void executeTestPlan()
{
    TestPlan plan;

    plan.addTestSuite(stringTestSuite());
    plan.addTestSuite(integerTestSuite());

    plan.execute();
}