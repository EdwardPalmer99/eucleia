/**
 * @file EucleiaUtility.hpp
 * @author Edward Palmer
 * @date 2024-11-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include <cstdio>

// TODO : -write a logger class.

// clang-format off
#define EucleiaError(...)\
({\
	fprintf(stderr, "%s:%d:%s(): error: ", __FILE__, __LINE__, __func__);\
	fprintf(stderr, __VA_ARGS__);\
	fprintf(stderr, "\n");\
	exit(EXIT_FAILURE);\
})
// clang-format on