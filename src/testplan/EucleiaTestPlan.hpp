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

    int execute();

    void addTestSuite(TestSuite testSuite);

    static TestPlan loadTestSuites(const std::string & testplanDir);
    
    const std::string name;

protected:
    std::vector<TestSuite> testSuites;
};

#endif