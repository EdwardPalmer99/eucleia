/**
 * @file Logger.cpp
 * @author Anand Doshi
 * @date 2025-01-08
 *
 * @copyright Copyright (c) 2025
 *
 */


#include "Logger.hpp"
#include "Exceptions.hpp"
#include "Stringify.hpp"
#include <cstdio>
#include <ctime>

Logger::Logger(Level thresholdLevel, std::ostream &logStream)
    : thresholdLevel(thresholdLevel), logStream(logStream)
{
}


Logger &Logger::instance()
{
    static Logger instance;

    return instance;
}


constexpr std::string Logger::levelName(Level level) const
{
    switch (level)
    {
        case Logger::Level::error:
            return "error";
        case Logger::Level::warning:
            return "warn ";
        case Logger::Level::info:
            return "info ";
        case Logger::Level::debug:
            return "debug";
        default:
            ThrowException("invalid Level enum");
    }
}

void Logger::log(Level level, std::string message) const
{
    if (!isLoggable(level))
        return;

    logStream << timestamp() << " " << levelName(level) << " " << message << std::endl;
}


std::string Logger::timestamp() const
{
    std::time_t now = std::time(nullptr);

    const std::size_t timestampFormatSize = std::size(Logger::timestampFormat);

    char timestamp[timestampFormatSize];
    std::strftime(timestamp, timestampFormatSize, "%FT%TZ", std::localtime(&now));

    // NB: will copy bytes in buffer.
    return std::string(timestamp);
}