//
//  EucleiaTestPlan.hpp
//  Eucleia
//
//  Created by Edward on 21/10/2024.
//

#ifndef EucleiaTestPlan_hpp
#define EucleiaTestPlan_hpp

#include <string>
#include <vector>
#include "EucleiaTestSuite.hpp"


class TestPlan
{
public:
    TestPlan() = default;

    void execute();

    void addTestSuite(TestSuite testSuite);
    
    const std::string name;
    const std::string description;

protected:
    std::vector<TestSuite> testSuites;
};

/**
 * @brief Runs a default testplan.
 */
void executeTestPlan();

#endif