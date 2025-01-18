/**
 * @file Logger.hpp
 * @author Anand Doshi
 * @date 2025-01-08
 *
 * @copyright Copyright (c) 2025
 *
 */


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
        error,
        warning,
        severe,
        info,
        fine,
        debug
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

    void setFilter(Filter filterToSet) { filter = filterToSet; }

    void log(Level level, std::string_view message);
    
    inline void error(std::string_view message) { log(Level::error, message); };
    inline void warning(std::string_view message) { log(Level::warning, message); };
    inline void severe(std::string_view message) { log(Level::severe, message); };
    inline void info(std::string_view message) { log(Level::info, message); };
    inline void fine(std::string_view message) { log(Level::fine, message); };
    inline void debug(std::string_view message) { log(Level::debug, message); };

private:
    std::string name;
    Filter filter;
    std::ostream &logStream;

    // ISO 8601 date time format
    inline static const std::string timestampFormat{"yyyy-mm-ddThh:mm:ssZ"};

    bool isLoggable(Level level);

    static constexpr std::string_view getLevelName(Level level);
};