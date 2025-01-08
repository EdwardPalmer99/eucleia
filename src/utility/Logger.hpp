/**
 * @file Logger.hpp
 * @author Anand Doshi
 * @date 2025-01-08
 *
 * @copyright Copyright (c) 2025
 *
 */


#include <cstdio>
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
    };

    Logger(std::string_view name)
      : Logger {name, Filter::important, std::cout}
    {
    }

    Logger(std::string_view name, Filter filter)
      : Logger {name, filter, std::cout}
    {
    }

    Logger(std::string_view name, Filter filter, ostream logStream)
      : name {name}
      , filter {filter}
      , logStream {logStream}
    {
    }

    void setFilter(Filter filterToSet) { filter = filterToSet; }

    void log(Level level, std::string_view message);
    void error(std::string_view message);
    void warning(std::string_view message);
    void severe(std::string_view message);
    void info(std::string_view message);
    void fine(std::string_view message);

  private:
    std::string name;
    Filter filter;
    ostream logStream;

    bool isLoggable(Level level);


}