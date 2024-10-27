//
//  main.cpp
//  Eucleia
//
//  Created by Edward on 12/11/2023.
//

#include <cstring>
#include <iostream>
#include <string>
#include <time.h>

#include "EucleiaInputStream.hpp"
#include "EucleiaInterpreter.hpp"
#include "EucleiaParser.hpp"
#include "EucleiaTokenizer.hpp"
#include "EucleiaUtility.hpp"

void printUsage(const char *argv[])
{
    printf("usage: %s [--help] [FILE]\n\n"
           "options:\n"
           "--help show this message and exit\n"
           "FILE   path to Eucleia file to run\n",
           argv[0]);
}


int main(int argc, const char *argv[])
{
    if (argc == 2)
    {
        if (strcmp(argv[1], "--help") == 0)
        {
            printUsage(argv);
            return EXIT_SUCCESS;
        }
        else
        {
            const char *fpath = argv[1];

            clock_t tick = clock();
            Interpreter::evaluateFile(std::string(fpath));
            clock_t tock = clock();

            fprintf(stdout, "run time: %d ms\n", (int)((1000.0 * (double)(tock - tick)) / (double)CLOCKS_PER_SEC));
            return EXIT_SUCCESS;
        }
    }
    else
    {
        printUsage(argv);
        return EXIT_FAILURE;
    }
}
