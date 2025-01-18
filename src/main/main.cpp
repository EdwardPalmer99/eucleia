//
//  main.cpp
//  Eucleia
//
//  Created by Edward on 12/11/2023.
//

#include "CLIParser.hpp"
#include "EucleiaInputStream.hpp"
#include "EucleiaInterpreter.hpp"
#include <exception>
#include <iostream>
#include <string>

int main(int argc, const char *argv[])
{
    try
    {
        CLIParser parser("eucleia");

        parser.addFlagArg("--help", "display available options");
        parser.addPositionalArg("fileName");
        parser.parseArgs(argc, argv);

        if (parser.isSet("--help") || !parser.isSet("fileName"))
        {
            parser.printOptions();
            return EXIT_SUCCESS;
        }

        Interpreter::evaluateFile(parser["fileName"]);
    }
    catch (std::exception &exception)
    {
        std::cout << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
