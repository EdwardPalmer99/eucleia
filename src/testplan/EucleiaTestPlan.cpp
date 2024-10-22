#include "EucleiaTestPlan.hpp"
#include "EucleiaUtility.hpp"
#include <dirent.h>
#include <cstring>


TestPlan TestPlan::loadTestSuites(const std::string & testplanDir)
{
    DIR *dir = opendir(testplanDir.c_str());
    if (!dir)
    {
        printWarpError("Failed to open directory %s\n", testplanDir.c_str());
    }

    struct dirent *entry = nullptr;

    TestPlan testplan;

    // TODO: - add safety checking. Handle case where we have a directory of
    // directories.
    char filePath[1024];

    while ((entry = readdir(dir)) != nullptr)
    {
        const char *fileName = entry->d_name;
        if (strcmp(fileName, ".") == 0 || strcmp(fileName, "..") == 0)
        {
            continue;
        }

        sprintf(filePath, "%s/%s", testplanDir.c_str(), fileName);

        testplan.addTestSuite(TestSuite::loadTestSuite(std::string(filePath)));
    }

    closedir(dir);

    return testplan;
}


void TestPlan::addTestSuite(TestSuite testSuite)
{
    testSuites.push_back(std::move(testSuite));
}


int TestPlan::execute()
{
    int testPlanStatus = EXIT_SUCCESS;

    for (auto & testSuite : testSuites)
    {
        int testSuiteStatus = testSuite.execute();

        if (testSuiteStatus != EXIT_SUCCESS)
        {
            testPlanStatus = EXIT_FAILURE;
        }
    }

    return testPlanStatus;
}