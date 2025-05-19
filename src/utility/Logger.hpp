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
#include "Stringify.hpp"
#include <condition_variable>
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

/* Add to top of functions for tracing */
#define _trace (Logger::instance().trace(eucleia::stringify("Entering: %s() [%s:%d]", __func__, __FILE__, __LINE__)))

enum class LogLevel
{
    Trace = 0,
    Debug = 1,
    Info = 2,
    Warning = 3,
    Error = 4,
    Critical = 5,
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

    void setThreshold(LogLevel threshold);

protected:
    friend class SingletonT<LoggerImpl>;

    /* Prevent direct initialization */
    LoggerImpl();

    ~LoggerImpl();

    void asyncLog(LogLevel level, std::string message);

    void log(LogLevel level, std::string message);

    bool isLoggable(LogLevel level) const;

    std::string timestamp() const;

    std::string levelName(LogLevel level) const;

private:
    friend void signalHandler(int signal);

    /* Thread loop */
    void loop();

    void shutdown();

    using Task = std::function<void()>;
    using Tasks = std::queue<Task>;

    using Lock = std::lock_guard<std::mutex>;

    LogLevel _threshold{LogLevel::Info};
    std::ofstream _fstream;

    std::thread _thread;
    Tasks _tasks;

    bool _shutdown{false};

    mutable std::mutex _mutex;
    mutable std::mutex _thresholdMutex;

    std::condition_variable _cv;

    /* ISO 8601 date time format */
    const std::string _timestampFormat{"%Y-%m-%dT%H:%M:%S"};

    const std::string _logPath{"/var/log/eucleia.log"};
};


using Logger = SingletonT<LoggerImpl>;


inline LoggerImpl &log()
{
    return Logger::instance();
}