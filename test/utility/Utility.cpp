/**
 * @file Utility.cpp
 * @author Edward Palmer
 * @date 2025-05-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Utility.hpp"
#include <cstdlib>

std::string getTestDirPath()
{
    static const std::string kTestSrcDir = std::string(getenv("TEST_SRCDIR"));

    return kTestSrcDir + "/_main/test/";
}
