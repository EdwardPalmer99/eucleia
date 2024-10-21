#include "EucleiaTestSuite.hpp"

void TestSuite::execute()
{
    fprintf(stdout, "\e[4mTestSuite: %-24s [%3lu tests]\e[0m\n", name.c_str(), testCases.size());

    for (auto & testCase : testCases)
    {
        testCase.execute();
    }
}


void TestSuite::addTestCase(const std::string name_, const std::string input_, const std::string expected_)
{
    auto testCase = TestCase(std::move(name_), std::move(input_), std::move(expected_));

    testCases.push_back(std::move(testCase));
}
