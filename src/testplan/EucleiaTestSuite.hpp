//
//  EucleiaTestSuite.hpp
//  Eucleia
//
//  Created by Edward on 21/10/2024.
//

#ifndef EucleiaTestSuite_hpp
#define EucleiaTestSuite_hpp

#include <string>
#include <vector>
#include "EucleiaTestCase.hpp"


class TestSuite
{
public:
    TestSuite() = delete;
    TestSuite(const std::string name_) : name(std::move(name_)) {}

    int execute();

    /**
     * @brief Load a test suite from a file.
     *
     * File format:
     *
     * @start: testSuiteName
     * <code here>
     * @end
     */
    static TestSuite loadTestSuite(const std::string &fpath);

    void addTestCase(const std::string name_, const std::string input_);
    
    const std::string name;

protected:
    std::vector<TestCase> testCases;
};

#endif