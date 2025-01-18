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
#include <string>

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

    static void debug(std::string message) { instance().log(Level::debug, std::move(message)); };
    static void info(std::string message) { instance().log(Level::info, std::move(message)); };
    static void warning(std::string message) { instance().log(Level::warning, std::move(message)); };
    static void error(std::string message) { instance().log(Level::error, std::move(message)); };

    // Returns non-const reference to singleton's thresholdLevel variable which enables it to be set.
    static Level &threshold() { return instance().thresholdLevel; }

protected:
    // Returns a singleton instance.
    static Logger &instance();

    Logger(Level thresholdLevel = Level::info, std::ostream &logStream = std::cout);

    // Logs a message at a given level.
    void log(Level level, std::string message) const;

    // Returns true if message should be printed to the log.
    inline bool isLoggable(Level level) const { return (level >= thresholdLevel); }

    // Returns a timestamp string using the timestamp format.
    std::string timestamp() const;

    // Returns a string for the level of the message.
    constexpr std::string levelName(Level level) const;

private:
    Level thresholdLevel;
    std::ostream &logStream;

    // ISO 8601 date time format
    inline static const std::string timestampFormat{"yyyy-mm-ddThh:mm:ssZ"};
};