#pragma once
#include "../all.h"

/**
 * Logs a formatted message with the INFO severity level, including source 
 * location and timestamp, to stdout.
 * 
 * @param message `printf()` style format string.
 * @param ... Optional format arguments.
 */
#define LOG_INFO(message, ...) log_message("INFO", __FILE__, __LINE__, message, ##__VA_ARGS__)

/**
 * Logs a formatted message with the WARNING severity level, including source 
 * location and timestamp, to stdout.
 * 
 * @param message `printf()` style format string.
 * @param ... Optional format arguments.
 */
#define LOG_WARNING(message, ...) log_message("WARNING", __FILE__, __LINE__, message, ##__VA_ARGS__)

/**
 * Logs a formatted message with the ERROR severity level, including source 
 * location and timestamp, to stdout.
 * 
 * @param message `printf()` style format string.
 * @param ... Optional format arguments.
 */
#define LOG_ERROR(message, ...) log_message("ERROR", __FILE__, __LINE__, message, ##__VA_ARGS__)

/**
 * Logs a message to stdout.
 * 
 * @param severity The severity level of the message.
 * @param file The file where the message originated.
 * @param line The line number where the message originated.
 * @param message The message to log, with optional format specifiers.
 * @param ... The arguments to format the message with.
 * 
 * @warning Not intended to be run directly, unless you have a good reason for
 * it. Use the `LOG_INFO`, `LOG_WARNING` and `LOG_ERROR` macros instead.
 */
void log_message(const char* severity, const char* file, int line, const char* message, ...);
