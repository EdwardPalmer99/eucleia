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

    static void debug(std::string_view message) { instance().log(Level::debug, message); };
    static void info(std::string_view message) { instance().log(Level::info, message); };
    static void warning(std::string_view message) { instance().log(Level::warning, message); };
    static void error(std::string_view message) { instance().log(Level::error, message); };

protected:
    static const Logger &instance();

    Logger(Level thresholdLevel = Level::info, std::ostream &logStream = std::cout);

    void log(Level level, std::string_view message) const;

    // Returns true if message should be printed to the log.
    inline bool isLoggable(Level level) const { return (level >= thresholdLevel); }

    // Returns a timestamp string using the timestamp format.
    std::string timestamp() const;

private:
    const Level thresholdLevel;
    std::ostream &logStream;

    // ISO 8601 date time format
    inline static const std::string timestampFormat{"yyyy-mm-ddThh:mm:ssZ"};

    static constexpr std::string_view getLevelName(Level level);
};