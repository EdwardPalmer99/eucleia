
#include <unistd.h>
#include <ostream>
#include <sys/wait.h>
#include "EucleiaTestCase.hpp"
#include "EucleiaInterpreter.hpp"
#include "EucleiaUtility.hpp"

int TestCase::execute()
{
    // Create a fork of the process so we can identify failures.
    int status;
    int pid = fork();

    if (pid == 0)   // Child -> execute.
    {
        // Evaluate. Did we get to the end without an error code?
        std::ostringstream out, err;
        Interpreter::evaluateString(input, out, err);
        exit(EXIT_SUCCESS);  // Okay.
    }
    else 
    {
        wait(&status);
    }

    fprintf(stdout, "%-40s %s\n", name.c_str(), 
            (status == 0) ? "\033[92mPASSED\033[0m" : "\033[91mFAILED\033[0m");

    return (status == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
