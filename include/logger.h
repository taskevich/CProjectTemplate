//
// Created by task on 17.10.2025.
//

#ifndef MYAPP_LOGGER_H
#define MYAPP_LOGGER_H
#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <mutex>
#include <chrono>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    Logger(const Logger &) = delete;

    Logger &operator=(const Logger &) = delete;

    static Logger &instance();

    void setLogFile(const std::string &path);

    void setLogLevel(LogLevel level);

    void log(LogLevel level, const std::string &message);

    void debug(const std::string &msg);

    void info(const std::string &msg);

    void warn(const std::string &msg);

    void error(const std::string &msg);

private:
    Logger() = default;

    ~Logger();

    static std::string currentTime();

    static std::string levelToString(LogLevel level);

private:
    std::ofstream logFile;
    std::mutex logMutex;
    LogLevel currentLevel = LogLevel::DEBUG;
};

#endif //MYAPP_LOGGER_H
