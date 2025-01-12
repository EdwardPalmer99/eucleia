/**
 * @file Exceptions.cpp
 * @author Edward Palmer
 * @date 2025-01-12
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Exceptions.hpp"
#include <cstdio>

GeneralException::GeneralException(const char *file, unsigned int line, const char *func, const std::string &message)
    : GeneralException(file, line, func, message.c_str())
{
}

GeneralException::GeneralException(const char *file, unsigned int line, const char *func, const char *message)
{
    char buffer[1024];
    snprintf(buffer, 1024, "%s:%d:%s(): %s", file, line, func, message);

    formattedMessage = std::string(buffer);
}

const char *GeneralException::what() const noexcept
{
    return formattedMessage.c_str();
}
