//
// Created by Rusher on 2021/12/6.
//


#pragma once

enum class LogLevel {
    L_ERROR,
    L_WARN,
    L_INFO,
    L_DEBUG,
};

class Log {
public:
    static void logMessage(LogLevel level, const char *format, ...);
};


#define LOG_DEBUG(format, ...) Log::logMessage(LogLevel::L_DEBUG, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) Log::logMessage(LogLevel::L_INFO, format, ##__VA_ARGS__)
#define LOG_WARN(format, ...) Log::logMessage(LogLevel::L_WARN, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) Log::logMessage(LogLevel::L_ERROR, format, ##__VA_ARGS__)