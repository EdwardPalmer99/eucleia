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
#include <sstream>

class Interpreter		// TODO: - just make a static class.
{
public:
	/// Prints output to std::out.
	static void evaluateFile(const std::string & fpath);
	
	/// Redirects std::out and returns a string containing all output.
	static std::string evaluateString(const std::string fileContents);

	/// Redirects stdout, stderr and evaluates.
	static void evaluateString(const std::string fileContents, std::ostringstream & out, std::ostringstream & err);
};

#endif /* EucleiaInterpreter_hpp */
