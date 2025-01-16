/**
 * @file Stringify.hpp
 * @author Edward Palmer
 * @date 2025-01-16
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include <string>

namespace eucleia
{
// Formats a C-style string with variadic arguments.
std::string stringify(const char *format, ...);
} // namespace eucleia
