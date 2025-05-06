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


LoggerImpl::LoggerImpl() : _thread(&LoggerImpl::loop, this)

{
    /* Register shutdown to be called automatically when program exits */
    std::atexit([]()
                { Logger::instance().shutdown(); });

    /* Register signal handlers */
    std::signal(SIGINT, signalHandler);  /* Handle Ctrl+C */
    std::signal(SIGTERM, signalHandler); /* Handle termination signals */
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

    if (_shutdown)
        return; /* Stop adding tasks (some lost log messages) */

    auto f = std::bind(&LoggerImpl::log, this, level, std::move(message));
    _tasks.push(std::move(f));
}


void LoggerImpl::log(LogLevel level, std::string message) const
{
    if (!isLoggable(level))
        return;

    _os << timestamp() << " " << levelName(level) << " " << message << std::endl;
}


void LoggerImpl::loop()
{
    while (true)
    {
        Task task;
        {
            Lock guard(_mutex);

            if (_shutdown && _tasks.empty()) /* Flush remaining and exit */
                return;
            else if (_tasks.empty())
                continue;

            task = std::move(_tasks.front());
            _tasks.pop();
        }

        task(); /* Execute */
    }
}


void LoggerImpl::shutdown()
{
    std::cout << "shutdown initiated..." << std::endl;

    {
        Lock guard(_mutex);
        _shutdown = true;
    }

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