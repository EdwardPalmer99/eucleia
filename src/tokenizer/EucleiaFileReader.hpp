//
//  EucleiaFileReader.hpp
//  Eucleia
//
//  Created by Edward on 20/10/2024.
//
//  Convert a file to a string.
//

#ifndef EucleiaFileReader_hpp
#define EucleiaFileReader_hpp

#include <string>
#include <cstdio>

namespace eucleia 
{

class EucleiaFileReader
{
public:
    /// Returns a std::string for a file path.
    std::string readFile(const std::string & fpath);

protected:
    /// Returns the size of a file in bytes.
    long fileSize(FILE * fp) const;
};


}

#endif  // EucleiaFileReader_hpp