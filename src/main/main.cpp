//
//  main.cpp
//  Eucleia
//
//  Created by Edward on 12/11/2023.
//

#include <time.h>
#include <string>
#include <cstring>
#include <iostream>

#include "EucleiaUtility.hpp"
#include "EucleiaInterpreter.hpp"
#include "EucleiaTokenizer.hpp"
#include "EucleiaInputStream.hpp"
#include "EucleiaParser.hpp"
#include "EucleiaTestPlan.hpp"
#include "EucleiaTestSuite.hpp"
#include "EucleiaTestCase.hpp"

void printUsage(const char *argv[])
{
	printf("usage: %s [--help] [--file FILE] [--testplan DIR] [--testsuite FILE]\n\n"
	       "options:\n"
	       "--help            show this message and exit\n"
	       "--file FILE       run Eucleia file\n"
	       "--testplan DIR    run testsuites in directory\n"
	       "--testsuite FILE  run testsuite\n",
			argv[0]);
}


int main(int argc, const char * argv[])
{
	if (argc == 2 && strcmp(argv[1], "--help") == 0)
	{
		printUsage(argv);
		return EXIT_SUCCESS;
	}
	else if (argc == 3 && strcmp(argv[1], "--file") == 0)
	{
		const char *fpath = argv[2];

		clock_t tick = clock();
		Interpreter::evaluateFile(std::string(fpath));
		clock_t tock = clock();
		
		fprintf(stdout, "run time: %d ms\n", (int)((1000.0 * (double)(tock - tick)) / (double)CLOCKS_PER_SEC));
		return EXIT_SUCCESS;
	}
	else if (argc == 3 && strcmp(argv[1], "--testplan") == 0)
	{
		const char *testPath = argv[2];

		TestPlan testplan = TestPlan::loadTestSuites(std::string(testPath));
		return testplan.execute();
	}
	else if (argc == 3 && strcmp(argv[1], "--testsuite") == 0)
	{
		const char *testPath = argv[2];

		TestSuite testSuite = TestSuite::loadTestSuite(std::string(testPath));
		return testSuite.execute();
	}
	else 
	{
		printUsage(argv);
		return EXIT_FAILURE;
	}
}
