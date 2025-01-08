/**
 * @file Logger.cpp
 * @author Anand Doshi
 * @date 2025-01-08
 *
 * @copyright Copyright (c) 2025
 *
 */


#include <cstdio>
#include <string>
#include <string_view>
#include <ctime>
#include "Logger.hpp"

bool Logger::isLoggable(Level level)
{
  if (filter == Filter::quiet) {
    return false;
  }

  if  (filter == Filter::important) {
    return level == Level::error 
          || level == Level::warning 
          || level == Level::severe;
  }

  if (filter == Filter::verbose) {
    return true;
  }

  return true; // unknown filter, everything logged
}

static constexpr std::string_view getLevelName(Level level)
{
  switch (level)
  {
    case Level::error: return "error";
    case Level::warning: return "warning";
    case Level::severe: return "severe";
    case Level::info: return "info";
    case Level::fine: return "fine";
    default: return level;
  }
}

void Logger::log(Level level, std::string_view message)
{
  if (isLoggable(level)) {

    std::time_t result = std::time(nullptr);
    std::string timestamp = std::asctime(std::localtime(&result));
    logStream  << '[' << timestamp << '] (' << getLevelName(level) << ') ' << message << '\n';
  }
}

void Logger::error(std::string_view message)
{
  log(Level::error, message);
}

void Logger::warning(std::string_view message)
{
  log(Level::warning, message);
}

void Logger::severe(std::string_view message)
{
  log(Level::severe, message);
}

void Logger::info(std::string_view message)
{
  log(Level::info, message);
}

void Logger::fine(std::string_view message)
{
  log(Level::fine, message);
}