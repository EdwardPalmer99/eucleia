/**
 * @file Exceptions.hpp
 * @author Edward Palmer
 * @date 2025-01-12
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include <stdexcept>

class GeneralException : public std::exception
{
public:
    GeneralException() = delete;

    GeneralException(const char *file, unsigned int line, const char *func, const std::string &message);

    GeneralException(const char *file, unsigned int line, const char *func, const char *message);

    const char *what() const noexcept override;

private:
    std::string formattedMessage;
};


#define ThrowException(message) \
    throw GeneralException(__FILE__, __LINE__, __func__, (message))
