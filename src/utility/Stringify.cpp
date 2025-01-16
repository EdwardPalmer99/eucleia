/**
 * @file Stringify.cpp
 * @author Edward Palmer
 * @date 2025-01-16
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Stringify.hpp"
#include <stdarg.h>

namespace eucleia
{
std::string stringify(const char *format, ...)
{
    if (!format)
    {
        return "";
    }

    char buffer[1024];

    va_list args;
    va_start(args, format);

    long nchars = vsnprintf(buffer, 1024, format, args);
    buffer[nchars] = '\0';

    va_end(args);

    // Copies from buffer and return std::string.
    return std::string(buffer);
}
} // namespace eucleia
