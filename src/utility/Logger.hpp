/**
 * @file Logger.hpp
 * @author Anand Doshi
 * @date 2025-01-08
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "SingletonT.hpp"
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>


enum class LogLevel
{
    Trace = 0,
    Debug = 1,
    Info = 2,
    Warning = 3,
    Error = 4,
    Critical = 5
};


class LoggerImpl
{
public:
    inline void trace(std::string message) { asyncLog(LogLevel::Trace, std::move(message)); }
    inline void debug(std::string message) { asyncLog(LogLevel::Debug, std::move(message)); }
    inline void info(std::string message) { asyncLog(LogLevel::Info, std::move(message)); }
    inline void warning(std::string message) { asyncLog(LogLevel::Warning, std::move(message)); }
    inline void error(std::string message) { asyncLog(LogLevel::Error, std::move(message)); }
    inline void critical(std::string message) { asyncLog(LogLevel::Critical, std::move(message)); }

    inline void setThreshold(LogLevel threshold) { _threshold = threshold; }

protected:
    friend class SingletonT<LoggerImpl>;

    /* Prevent direct initialization */
    LoggerImpl();

    ~LoggerImpl();

    void asyncLog(LogLevel level, std::string message);

    void log(LogLevel level, std::string message) const;

    inline bool isLoggable(LogLevel level) const { return (level >= _threshold); }

    std::string timestamp() const;

    std::string levelName(LogLevel level) const;

private:
    friend void signalHandler(int signal);

    /* Thread loop */
    void loop();

    void shutdown();

    using Task = std::function<void()>;
    using Tasks = std::queue<Task>;

    LogLevel _threshold{LogLevel::Info};
    std::ostream &_os{std::cout};

    std::thread _thread;

    Tasks _tasks;

    bool _shutdown{false};

    /* ISO 8601 date time format */
    const std::string _timestampFormat{"yyyy-mm-ddThh:mm:ssZ"};

    using Lock = std::lock_guard<std::mutex>;

    mutable std::mutex _mutex;
};


using Logger = SingletonT<LoggerImpl>;


inline LoggerImpl &log()
{
    return Logger::instance();
}