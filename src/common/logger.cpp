#include "logger.h"

Logger &Logger::instance() {
    static Logger instance;
    return instance;
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::setLogFile(const std::string &path) {
    std::lock_guard<std::mutex> lock(logMutex);
    if (logFile.is_open()) logFile.close();
    logFile.open(path, std::ios::app);
}

void Logger::setLogLevel(const LogLevel level) {
    std::lock_guard<std::mutex> lock(logMutex);
    currentLevel = level;
}

void Logger::log(const LogLevel level, const std::string &message) {
    if (level < currentLevel) return;

    std::lock_guard<std::mutex> lock(logMutex);
    const std::string logMsg = "[" + currentTime() + "] [" + levelToString(level) + "] " + message;

    std::cout << logMsg << std::endl;

    if (logFile.is_open()) {
        logFile << logMsg << std::endl;
    }
}

void Logger::debug(const std::string &msg) { log(LogLevel::DEBUG, msg); }
void Logger::info(const std::string &msg) { log(LogLevel::INFO, msg); }
void Logger::warn(const std::string &msg) { log(LogLevel::WARNING, msg); }
void Logger::error(const std::string &msg) { log(LogLevel::ERROR, msg); }

std::string Logger::levelToString(const LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARN";
        case LogLevel::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

std::string Logger::currentTime() {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto time = system_clock::to_time_t(now);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &time);
#else
    localtime_r(&time, &tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
