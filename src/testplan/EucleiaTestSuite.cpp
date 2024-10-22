#include "EucleiaTestSuite.hpp"
#include "EucleiaUtility.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <iostream>

int TestSuite::execute()
{
    fprintf(stdout, "\e[4mTestSuite: %-24s [%3lu tests]\e[0m\n", name.c_str(), testCases.size());

    int testSuiteStatus = EXIT_SUCCESS;

    for (auto & testCase : testCases)
    {
        int testCaseStatus = testCase.execute();
        if (testCaseStatus != EXIT_SUCCESS)
        {
            testSuiteStatus = EXIT_FAILURE;
        }
    }

    return testSuiteStatus;
}

TestSuite TestSuite::loadTestSuite(const std::string & fpath)
{
    FILE *fp = fopen(fpath.c_str(), "r");
    if (!fp)
    {
        printWarpError("error: failed to open file %s\n", fpath.c_str());
    }

    char *testSuiteName = basename((char *)fpath.c_str());
    TestSuite suite = TestSuite(std::string(testSuiteName));

    size_t kBufferSize = 1024;
    char *buffer = (char *)malloc(sizeof(char) * kBufferSize);

    char testCaseName[kBufferSize];
    char testCaseInput[kBufferSize];
    int testCaseLen = 0;

    bool isReadingTestCase = false;

    while ((getline(&buffer, &kBufferSize, fp) > 0))
    {
        if (!isReadingTestCase && strncmp(buffer, "@start", 6) == 0)
        {
            sscanf(buffer, "@start %s\n", testCaseName);
            isReadingTestCase = true;
            testCaseLen = 0;
            continue;
        }
        
        if (isReadingTestCase && strncmp(buffer, "@end", 4) == 0)
        {
            suite.addTestCase(std::string(testCaseName), std::string(testCaseInput));

            isReadingTestCase = false;
            testCaseLen = 0;
            continue;
        }

        if (isReadingTestCase)
        {
            strcpy(testCaseInput + testCaseLen, buffer);
            testCaseLen += strlen(buffer);
            continue;
        }
    }

    free(buffer);
    fclose(fp);

    return suite;
}


void TestSuite::addTestCase(const std::string name_, const std::string input_)
{
    auto testCase = TestCase(std::move(name_), std::move(input_));

    testCases.push_back(std::move(testCase));
}
