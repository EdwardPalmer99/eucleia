//
//  EucleiaUtility.hpp
//  Eucleia
//
//  Created by Edward on 28/01/2024.
//

#ifndef EucleiaUtility_hpp
#define EucleiaUtility_hpp

#include <stdio.h>

// TODO: - throw C++ exception instead.

// clang-format off
#define EucleiaError(fmt, args...)\
({\
	fprintf(stderr, "%s:%d:%s(): error: " fmt, __FILE__, __LINE__, __func__, args);\
	exit(EXIT_FAILURE);\
})
// clang-format on

#endif /* EucleiaUtility_hpp */
