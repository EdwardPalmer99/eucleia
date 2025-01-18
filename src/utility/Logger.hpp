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

    Logger(std::string_view name, Level thresholdLevel = Level::info, std::ostream &logStream = std::cout);
    Logger() = delete;

    inline void debug(std::string_view message) const { log(Level::debug, message); };
    inline void info(std::string_view message) const { log(Level::info, message); };
    inline void warning(std::string_view message) const { log(Level::warning, message); };
    inline void error(std::string_view message) const { log(Level::error, message); };

protected:
    void log(Level level, std::string_view message) const;

private:
    const std::string name;
    const Level thresholdLevel;
    std::ostream &logStream;

    // ISO 8601 date time format
    inline static const std::string timestampFormat{"yyyy-mm-ddThh:mm:ssZ"};

    bool isLoggable(Level level) const;

    static constexpr std::string_view getLevelName(Level level);
};