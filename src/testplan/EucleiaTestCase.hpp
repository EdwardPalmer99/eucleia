//
//  EucleiaTestCase.hpp
//  Eucleia
//
//  Created by Edward on 21/10/2024.
//

#ifndef EucleiaTestCase_hpp
#define EucleiaTestCase_hpp

#include <string>

class TestCase
{
public:
    TestCase() = delete;

    explicit TestCase(const std::string name_, const std::string input_) 
        : name(std::move(name_)), input(std::move(input_)) {}

    int execute();
    
    const std::string name;
    const std::string input;
};

#endif