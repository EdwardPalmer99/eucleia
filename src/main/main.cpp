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

int main(int argc, const char * argv[])
{
	if (argc == 2 && strcmp(argv[1], "--help") == 0)
	{
		printf("usage: %s [file] [options]\noptions:\n%s\n",
				argv[0],
			   "");
		return EXIT_SUCCESS;
	}
		
	const char *fpath = nullptr;
	
	for (int i = 1; i < argc; i++)	// Skip execuatable path.
	{
		const char *argument = argv[i];

		if (!fpath)
			fpath = argument;
		else
			printWarpError("unknown argument '%s'.\n", argument);
	}
	
	if (!fpath)
	{
		printWarpError("%s", "missing file path.\n");
	}
	
	clock_t tick = clock();
	Interpreter::evaluateFile(std::string(fpath));
	clock_t tock = clock();
	
	fprintf(stdout, "run time: %d ms\n", (int)((1000.0 * (double)(tock - tick)) / (double)CLOCKS_PER_SEC));
	return EXIT_SUCCESS;
}
