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
    TestSuite(const std::string name_, const std::string description_ = "") 
        : name(std::move(name_)), description(std::move(description_)) {}

    void execute();

    void addTestCase(const std::string name_, const std::string input_, const std::string expected_);
    
    const std::string name;
    const std::string description;

protected:
    std::vector<TestCase> testCases;
};

#endif