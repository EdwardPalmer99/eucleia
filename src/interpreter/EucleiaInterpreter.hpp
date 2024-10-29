//
//  EucleiaInterpreter.hpp
//  Eucleia
//
//  Created by Edward on 25/01/2024.
//

#ifndef EucleiaInterpreter_hpp
#define EucleiaInterpreter_hpp

#include <sstream>
#include <string>
#include <vector>

class Interpreter // TODO: - just make a static class.
{
  public:
    /// Prints output to std::out.
    static void evaluateFile(const std::string &fpath);

    /// Redirects std::out and returns a string containing all output.
    static std::string evaluateString(const std::string fileContents);  
};

#endif /* EucleiaInterpreter_hpp */
