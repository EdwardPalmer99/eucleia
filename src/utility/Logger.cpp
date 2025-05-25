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
#include <chrono>
#include <csignal>
#include <cstdio>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

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

    _fstream = std::ofstream(_logPath, std::fstream::out);
    if (!_fstream.is_open())
    {
        std::cerr << "Logger failed to initialize with path: " << _logPath << std::endl;
    }
    else
    {
        std::cout << "Logger initialized with path: " << _logPath << std::endl;
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
            return "Trace";
        case LogLevel::Debug:
            return "Debug";
        case LogLevel::Info:
            return "Info";
        case LogLevel::Warning:
            return "Warn";
        case LogLevel::Error:
            return "Error";
        case LogLevel::Critical:
            return "Critical";
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
    auto now = std::chrono::system_clock::now();

    time_t now_t = std::chrono::system_clock::to_time_t(now); /* Convert */

    /* Extract microseconds */
    auto now_micro = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now_t), _timestampFormat.c_str()) << "." << std::setfill('0') << std::setw(6) << now_micro.count();

    return oss.str();
}


void signalHandler(int signal)
{
    std::cerr << "Signal received: " << signal << ". Shutting down logger..." << std::endl;

    Logger::instance().shutdown();

    std::exit(signal); /* Exit program after cleanup */
}