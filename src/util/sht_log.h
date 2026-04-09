/*****      log - Logging utilities      *****/
/***** https://codeberg.org/zoomlogo/sht *****/
// Version: v0.1
#ifndef SHT_LOG_DEFINED_H
#define SHT_LOG_DEFINED_H

#include <stdio.h>
#include <string.h>

// Define SHT_LOG_NO_COLORS to disable colors in the output.
// Set SHT_LOG_LEVEL to set the log level.  The log level is in the following:
// By default the log level is SHT_LOG_NONE, which means no output is observed.
// 1. SHT_LOG_DEBUG
// 2. SHT_LOG_INFO
// 3. SHT_LOG_WARN
// 4. SHT_LOG_ERROR.
// 5. SHT_LOG_NONE.

#ifndef SHT_LOG_NO_COLORS
#define SHT_LOG_RED "\033[31m"
#define SHT_LOG_GREEN "\033[32m"
#define SHT_LOG_YELLOW "\033[33m"
#define SHT_LOG_BLUE "\033[34m"
#define SHT_LOG_RESET "\033[0m"
#else
#define SHT_LOG_RED
#define SHT_LOG_GREEN
#define SHT_LOG_YELLOW
#define SHT_LOG_BLUE
#define SHT_LOG_RESET
#endif

#ifdef SHT_STRIP_PREFIX
#define log_debug sht_log_debug
#define log_info sht_log_info
#define log_warn sht_log_warn
#define log_error sht_log_error
#endif

#define SHT_LOG_DEBUG 0
#define SHT_LOG_INFO 1
#define SHT_LOG_WARN 2
#define SHT_LOG_ERROR 3
#define SHT_LOG_NONE 4

#define __FNAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define sht_log_with(level, fmt, ...) \
        fprintf(stderr, "[%s] %s:%d: " fmt "\n", level, __FNAME__, __LINE__ __VA_OPT__(,) __VA_ARGS__);

#ifndef SHT_LOG_LEVEL
#define SHT_LOG_LEVEL SHT_LOG_NONE
#endif

#if SHT_LOG_LEVEL <= SHT_LOG_DEBUG
#define sht_log_debug(fmt, ...) \
        sht_log_with(SHT_LOG_BLUE "DEBUG" SHT_LOG_RESET, fmt __VA_OPT__(,) __VA_ARGS__)
#else
#define sht_log_debug(fmt, ...) do {} while (0)
#endif

#if SHT_LOG_LEVEL <= SHT_LOG_INFO
#define sht_log_info(fmt, ...) \
        sht_log_with(SHT_LOG_GREEN "INFO" SHT_LOG_RESET, fmt __VA_OPT__(,) __VA_ARGS__)
#else
#define sht_log_info(fmt, ...) do {} while (0)
#endif

#if SHT_LOG_LEVEL <= SHT_LOG_WARN
#define sht_log_warn(fmt, ...) \
        sht_log_with(SHT_LOG_YELLOW "WARN" SHT_LOG_RESET, fmt __VA_OPT__(,) __VA_ARGS__)
#else
#define sht_log_warn(fmt, ...) do {} while (0)
#endif

#if SHT_LOG_LEVEL <= SHT_LOG_ERROR
#define sht_log_error(fmt, ...) \
        sht_log_with(SHT_LOG_RED "ERROR" SHT_LOG_RESET, fmt __VA_OPT__(,) __VA_ARGS__)
#else
#define sht_log_error(fmt, ...) do {} while (0)
#endif

#endif  // SHT_LOG_DEFINED_H
