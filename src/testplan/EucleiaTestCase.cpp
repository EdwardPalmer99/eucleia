#include "EucleiaTestCase.hpp"
#include "EucleiaInterpreter.hpp"
#include "EucleiaUtility.hpp"

void TestCase::execute()
{
    const std::string output = Interpreter::evaluateString(input);

    bool pass = (output == expected);

    fprintf(stdout, "%-40s %s\n", name.c_str(), 
            pass ? "\033[92mPASSED\033[0m" : "\033[91mFAILED\033[0m");

    if (!pass)
    {
        exit(EXIT_FAILURE);
    }
}
