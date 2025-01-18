/**
 * @file Logger.cpp
 * @author Anand Doshi
 * @date 2025-01-08
 *
 * @copyright Copyright (c) 2025
 *
 */


#include "Logger.hpp"
#include <cstdio>
#include <ctime>
#include <iterator>
#include <stdexcept>
#include <string>
#include <string_view>

bool Logger::isLoggable(Level level)
{
    if (filter == Filter::quiet)
    {
        return false;
    }

    if (filter == Filter::important)
    {
        return level == Level::error || level == Level::warning || level == Level::severe;
    }

    if (filter == Filter::verbose)
    {
        return true;
    }

    return true; // unknown filter, everything logged
}

constexpr std::string_view Logger::getLevelName(Logger::Level level)
{
    switch (level)
    {
        case Logger::Level::error:
            return "error";
        case Logger::Level::warning:
            return "warning";
        case Logger::Level::severe:
            return "severe";
        case Logger::Level::info:
            return "info";
        case Logger::Level::fine:
            return "fine";
        case Logger::Level::debug:
            return "debug";
        default:
            throw std::invalid_argument("Invalid Level enum.");
    }
}

void Logger::log(Level level, std::string_view message)
{
    if (isLoggable(level))
    {

        std::time_t now = std::time(nullptr);

        char timestamp[std::size(Logger::timestampFormat)];
        std::strftime(timestamp, std::size(Logger::timestampFormat),
                      "%FT%TZ", std::localtime(&now));
        logStream << '[' << timestamp << "] " << '(' << name << ") " << '[' << getLevelName(level) << "] " << message << '\n';
    }
}