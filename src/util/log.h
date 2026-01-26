/**
 * @file log.h
 * @brief Logging utilities.
 */
#pragma once
#include <stdio.h>
#include <string.h>

#define LOG_DEBUG 0
#define LOG_INFO 1
#define LOG_WARN 2
#define LOG_ERROR 3
#define LOG_NONE 4

#define __FNAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define log_with(level, fmt, ...) fprintf(stderr, "[%s] %s:%d: " fmt "\n", level, __FNAME__, __LINE__, ##__VA_ARGS__);

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_NONE
#endif

#if LOG_LEVEL <= LOG_DEBUG
#define log_debug(fmt, ...) log_with("DEBUG", fmt, ##__VA_ARGS__)
#else
#define log_debug(fmt, ...) do {} while (0)
#endif

#if LOG_LEVEL <= LOG_INFO
#define log_info(fmt, ...) log_with("INFO", fmt, ##__VA_ARGS__)
#else
#define log_info(fmt, ...) do {} while (0)
#endif

#if LOG_LEVEL <= LOG_WARN
#define log_warn(fmt, ...) log_with("WARN", fmt, ##__VA_ARGS__)
#else
#define log_warn(fmt, ...) do {} while (0)
#endif

#if LOG_LEVEL <= LOG_ERROR
#define log_error(fmt, ...) log_with("ERROR", fmt, ##__VA_ARGS__)
#else
#define log_error(fmt, ...) do {} while (0)
#endif
