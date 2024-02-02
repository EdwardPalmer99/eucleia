//
//  main.cpp
//  Eucleia
//
//  Created by Edward on 12/11/2023.
//

#include <time.h>
#include <string>
#include <iostream>

#include "EucleiaUtility.hpp"
#include "EucleiaInterpreter.hpp"
#include "EucleiaTokenizer.hpp"
#include "EucleiaInputStream.hpp"
#include "EucleiaParser.hpp"

int main(int argc, const char * argv[])
{
	std::string fpath = "/Users/edward/Desktop/test.txt";
	
	clock_t tick = clock();
	Interpreter::evaluateFile(fpath);
	clock_t tock = clock();
	fprintf(stdout, "run time: %d ms\n", (int)((1000.0 * (double)(tock - tick)) / (double)CLOCKS_PER_SEC));

	
	
	

	
//	if (argc == 2 && strcmp(argv[1], "--help") == 0)
//	{
//		printf("usage: %s [file] [options]\noptions:\n%s\n",
//				argv[0],
//			   "--print-tokens");
//		return EXIT_SUCCESS;
//	}
//		
//	const char *fpath 	= NULL;
//	bool debugTokenizer	= false;
//	
//	for (int i = 1; i < argc; i++)	// Skip execuatable path.
//	{
//		const char *argument = argv[i];
//		
//		if (strcmp(argument, "--print-tokens") == 0)
//			debugTokenizer = true;
//		else if (!fpath)
//			fpath = argument;
//		else
//			printWarpError("unknown argument '%s'.\n", argument);
//	}
//	
//	//fpath = "/Users/edward/Desktop/tests/structDefinition.warp";//"/Users/edward/Desktop/tests/printFib.warp";
//
//	if (!fpath)
//	{
//		printWarpError("%s", "missing file path.\n");
//	}
//	
//	// Run with options.
////	if (debugTokenizer)
////	{
////		runTokenizerTestFile(fpath);
////	}
//	
//	clock_t tick = clock();
//	Interpreter::runFile(std::string(fpath));
//	clock_t tock = clock();
//
//	fprintf(stdout, "run time: %d ms\n", (int)((1000.0 * (double)(tock - tick)) / (double)CLOCKS_PER_SEC));
//	
//	
//	return EXIT_SUCCESS;
}
