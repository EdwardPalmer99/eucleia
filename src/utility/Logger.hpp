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
    enum class Filter
    {
        quiet,
        verbose,
        important
    };

    enum class Level
    {
        debug,
        info,
        warning,
        error
    };

    Logger(std::string_view name)
        : Logger{name, Filter::important, std::cout}
    {
    }

    Logger(std::string_view name, Filter filter)
        : Logger{name, filter, std::cout}
    {
    }

    Logger(std::string_view name, Filter filter, std::ostream &logStream)
        : name{name}, filter{filter}, logStream{logStream}
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
    const Filter filter;
    std::ostream &logStream;

    // ISO 8601 date time format
    inline static const std::string timestampFormat{"yyyy-mm-ddThh:mm:ssZ"};

    bool isLoggable(Level level);

    static constexpr std::string_view getLevelName(Level level);
};