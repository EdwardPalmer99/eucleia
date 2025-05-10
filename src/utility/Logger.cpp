/**
 * @file Logger.cpp
 * @author Anand Doshi
 * @date 2025-01-08
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Logger.hpp"
#include "Exceptions.hpp"
#include <csignal>
#include <cstdio>
#include <ctime>
#include <iostream>


void signalHandler(int signal);


void LoggerImpl::setThreshold(LogLevel threshold)
{
    Lock lock(_thresholdMutex);
    _threshold = threshold;
}


bool LoggerImpl::isLoggable(LogLevel level) const
{
    Lock lock(_thresholdMutex); /* TODO: - this will be inefficient. Implement ReaderWriterLock */
    return (level >= _threshold);
}


LoggerImpl::LoggerImpl()
{
    /* Register shutdown to be called automatically when program exits */
    std::atexit([]()
    { Logger::instance().shutdown(); });

    /* Register signal handlers */
    std::signal(SIGINT, signalHandler);  /* Handle Ctrl+C */
    std::signal(SIGTERM, signalHandler); /* Handle termination signals */

    _fstream = std::ofstream("/var/log/eucleia.log", std::fstream::out | std::fstream::app);
    if (!_fstream.is_open())
    {
        std::cerr << "Logger failed to initialize with path: " << _logPath << std::endl;
    }
    else
    {
        _thread = std::thread(&LoggerImpl::loop, this); /* Startup logging loop */
    }
}


LoggerImpl::~LoggerImpl()
{
    shutdown();
}


std::string LoggerImpl::levelName(LogLevel level) const
{
    switch (level)
    {
        case LogLevel::Trace:
            return "trace";
        case LogLevel::Debug:
            return "debug";
        case LogLevel::Info:
            return "info";
        case LogLevel::Warning:
            return "warning";
        case LogLevel::Error:
            return "error";
        case LogLevel::Critical:
            return "critical";
        default:
            ThrowException("invalid Level enum");
    }
}


void LoggerImpl::asyncLog(LogLevel level, std::string message)
{
    Lock guard(_mutex);

    if (_shutdown || !_fstream.is_open())
        return; /* Stop adding tasks (some lost log messages) */

    auto f = std::bind(&LoggerImpl::log, this, level, std::move(message));
    _tasks.push(std::move(f));
    _cv.notify_one();
}


void LoggerImpl::log(LogLevel level, std::string message)
{
    if (!isLoggable(level))
        return;

    _fstream << timestamp() << " " << levelName(level) << " " << message << std::endl;
}


void LoggerImpl::loop()
{
    while (true)
    {
        Task task;
        {
            std::unique_lock<std::mutex> lock(_mutex);

            _cv.wait(lock, [this]()
            { return (!_tasks.empty() || _shutdown); });

            if (_shutdown && _tasks.empty()) /* Flush remaining and exit */
                return;

            task = std::move(_tasks.front());
            _tasks.pop();
        }

        task(); /* Execute */
    }
}


void LoggerImpl::shutdown()
{
    {
        Lock guard(_mutex);
        _shutdown = true;
    }

    _cv.notify_all(); /* Wakeup thread */
    if (_thread.joinable())
    {
        _thread.join(); /* Only return once thread finishes */
    }
}


std::string LoggerImpl::timestamp() const
{
    std::time_t now = std::time(nullptr);

    const std::size_t timestampFormatSize = std::size(_timestampFormat);

    char timestamp[timestampFormatSize];
    std::strftime(timestamp, timestampFormatSize, "%FT%TZ", std::localtime(&now));

    // NB: will copy bytes in buffer.
    return std::string(timestamp);
}


void signalHandler(int signal)
{
    std::cerr << "Signal received: " << signal << ". Shutting down logger..." << std::endl;

    Logger::instance().shutdown();

    std::exit(signal); /* Exit program after cleanup */
}