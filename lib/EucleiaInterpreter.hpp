//
//  EucleiaInterpreter.hpp
//  Eucleia
//
//  Created by Edward on 25/01/2024.
//

#ifndef EucleiaInterpreter_hpp
#define EucleiaInterpreter_hpp

#include <string>
#include <vector>

class Interpreter		// TODO: - just make a static class.
{
public:
	/// Prints output to std::out.
	static void evaluateFile(const std::string & fpath);
	
	/// Redirects std::out and return s a string containing all output.
	static std::string evaluateTestFile(const std::string & fpath);
};

#endif /* EucleiaInterpreter_hpp */
