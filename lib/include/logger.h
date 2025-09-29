#ifndef DISFS_LOGGER_H_
#define DISFS_LOGGER_H_

#include <stdio.h>

#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define PURPLE "\x1B[35m"
#define BLUE "\x1B[36m"
#define YELLOW "\x1B[90m"
#define RESET "\x1B[0m"

#define _log(stream, file, line, level, ...)                                   \
    do                                                                         \
    {                                                                          \
        fprintf(stream, "%s %s:%d  --  ", level, file, line);                  \
        fprintf(stream, __VA_ARGS__);                                          \
    } while (0)

#define LOG_ERROR(...)                                                         \
    _log(stderr, __FILE__, __LINE__, RED "[ERROR] " RESET, __VA_ARGS__)
#define LOG_WARNING(...)                                                       \
    _log(stderr, __FILE__, __LINE__, YELLOW "[WARNING] " RESET, __VA_ARGS__)
#define LOG_TRACE(...)                                                         \
    _log(stderr, __FILE__, __LINE__, BLUE "[TRACE] " RESET, __VA_ARGS__)
#define LOG_DEBUG(...)                                                         \
    _log(stderr, __FILE__, __LINE__, PURPLE "[DEBUG] " RESET, __VA_ARGS__)
#define LOG_INFO(...)                                                          \
    _log(stderr, __FILE__, __LINE__, GREEN "[INFO] " RESET, __VA_ARGS__)

#endif
