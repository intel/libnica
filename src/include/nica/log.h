/*
 * This file is part of libnica.
 *
 * Copyright © 2016-2017 Intel Corporation
 *
 * libnica is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 */

#pragma once

#define _GNU_SOURCE

#include <stdio.h>

/**
 * Pre-defined log levels for the Nica logging API.
 * The default log level is set to NC_LOG_LEVEL_LEVEL_WARNING, so that only
 * errors and above are display.
 * The NC_DEBUG environmental variable can be used to control this.
 */
typedef enum {
        NC_LOG_LEVEL_DEBUG = 0, /**< NC_DEBUG=1 */
        NC_LOG_LEVEL_INFO,      /**< NC_DEBUG=2 */
        NC_LOG_LEVEL_SUCCESS,   /**< NC_DEBUG=3 */
        NC_LOG_LEVEL_WARNING,   /**< NC_DEBUG=4 */
        NC_LOG_LEVEL_ERROR,     /**< NC_DEBUG=5 */
        NC_LOG_LEVEL_FATAL,     /**< NC_DEBUG=6 */
        NC_LOG_LEVEL_MAX        /* Unused */
} NcLogLevel;

/**
 * Re-initialise the logging functionality, to use a different file descriptor
 * for logging
 *
 * @note This is already called once with stderr as the log file
 */
void nc_log_init(FILE *log);

/**
 * Override the default log level used by the Nica logging API
 */
void nc_log_set_level(NcLogLevel level);

/**
 * Log current status/error to the log file, which defaults to stderr.
 * Convenience macros are provided to simplify logging.
 */
void nc_log(NcLogLevel level, const char *file, int line, const char *format, ...)
    __attribute__((format(printf, 4, 5)));

/**
 * Log a simple debug message
 */
#define NC_LOG_DEBUG(...) (nc_log(NC_LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__))

/**
 * Log an informational message
 */
#define NC_LOG_INFO(...) (nc_log(NC_LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__))

/**
 * Log success
 */
#define NC_LOG_SUCCESS(...) (nc_log(NC_LOG_LEVEL_SUCCESS, __FILE__, __LINE__, __VA_ARGS__))

/**
 * Log a warning message that must always be seen
 */
#define NC_LOG_WARNING(...) (nc_log(NC_LOG_LEVEL_WARNING, __FILE__, __LINE__, __VA_ARGS__))

/**
 * Log a non-fatal error
 */
#define NC_LOG_ERROR(...) (nc_log(NC_LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__))

/**
 * Log a fatal error
 */
#define NC_LOG_FATAL(...) (nc_log(NC_LOG_LEVEL_FATAL, __FILE__, __LINE__, __VA_ARGS__))

/*
 * Editor modelines  -  https://www.wireshark.org/tools/modelines.html
 *
 * Local variables:
 * c-basic-offset: 8
 * tab-width: 8
 * indent-tabs-mode: nil
 * End:
 *
 * vi: set shiftwidth=8 tabstop=8 expandtab:
 * :indentSize=8:tabSize=8:noTabs=true:
 */
