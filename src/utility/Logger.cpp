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
#include <cstdio>
#include <ctime>
#include <iterator>
#include <stdexcept>
#include <string>
#include <string_view>

Logger::Logger(std::string_view name, Level thresholdLevel, std::ostream &logStream)
    : name(name), thresholdLevel(thresholdLevel), logStream(logStream)
{
}


const Logger &Logger::instance()
{
    static Logger instance("default");

    return instance;
}


bool Logger::isLoggable(Level level) const
{
    return (level >= thresholdLevel);
}

constexpr std::string_view Logger::getLevelName(Logger::Level level)
{
    switch (level)
    {
        case Logger::Level::error:
            return "error";
        case Logger::Level::warning:
            return "warning";
        case Logger::Level::info:
            return "info";
        case Logger::Level::debug:
            return "debug";
        default:
            ThrowException("Invalid Level enum.");
    }
}

void Logger::log(Level level, std::string_view message) const
{
    if (!isLoggable(level))
        return;

    std::time_t now = std::time(nullptr);

    char timestamp[std::size(Logger::timestampFormat)];
    std::strftime(timestamp, std::size(Logger::timestampFormat),
                  "%FT%TZ", std::localtime(&now));

    char outputInfo[1024];
    snprintf(outputInfo, 1024, "[%s](%s)[%s] ", timestamp, name.c_str(), getLevelName(level).data());
    logStream << outputInfo << message << '\n';
}