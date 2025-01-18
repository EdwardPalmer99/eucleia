/**
 * @file Logger.hpp
 * @author Anand Doshi
 * @date 2025-01-08
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <iostream>
#include <string_view>

class Logger
{
public:
    enum class Level
    {
        debug,
        info,
        warning,
        error
    };

    Logger(std::string_view name)
        : Logger(name, Level::info, std::cout)
    {
    }

    Logger(std::string_view name, Level thresholdLevel)
        : Logger(name, thresholdLevel, std::cout)
    {
    }

    Logger(std::string_view name, Level thresholdLevel, std::ostream &logStream)
        : name(name), thresholdLevel(thresholdLevel), logStream(logStream)
    {
    }

    Logger() = delete;

    void log(Level level, const char *file, unsigned int line, const char *func, std::string_view message);

    inline void error(const char *file, unsigned int line, const char *func, std::string_view message) { log(Level::error, file, line, func, message); };
    inline void warning(const char *file, unsigned int line, const char *func, std::string_view message) { log(Level::warning, file, line, func, message); };
    inline void info(const char *file, unsigned int line, const char *func, std::string_view message) { log(Level::info, file, line, func, message); };
    inline void debug(const char *file, unsigned int line, const char *func, std::string_view message) { log(Level::debug, file, line, func, message); };

private:
    const std::string name;
    const Level thresholdLevel;
    std::ostream &logStream;

    // ISO 8601 date time format
    inline static const std::string timestampFormat{"yyyy-mm-ddThh:mm:ssZ"};

    bool isLoggable(Level level);

    static constexpr std::string_view getLevelName(Level level);
};