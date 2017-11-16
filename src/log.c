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

#define _GNU_SOURCE

#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "util.h"

static FILE *log_file = NULL;
static NcLogLevel min_log_level;

static const char *log_str_table[] = {[NC_LOG_LEVEL_DEBUG] = "DEBUG",
                                      [NC_LOG_LEVEL_INFO] = "INFO",
                                      [NC_LOG_LEVEL_SUCCESS] = "SUCCESS",
                                      [NC_LOG_LEVEL_WARNING] = "WARNING",
                                      [NC_LOG_LEVEL_ERROR] = "ERROR",
                                      [NC_LOG_LEVEL_FATAL] = "FATAL" };

void nc_log_init(FILE *log)
{
        const char *env_level = NULL;
        log_file = log;
        unsigned int nlog_level = NC_LOG_LEVEL_WARNING;

        if (!log_file) {
                log_file = stderr;
        }

        env_level = getenv("NC_DEBUG");
        if (env_level) {
                /* =1 becomes 0 */
                nlog_level = ((unsigned int)atoi(env_level)) - 1;
        }
        nc_log_set_level(nlog_level);
}

void nc_log_set_level(NcLogLevel level)
{
        if (level >= NC_LOG_LEVEL_MAX) {
                NC_LOG_ERROR("Attempted to set invalid log level: %u", level);
                level = NC_LOG_LEVEL_FATAL;
        }
        min_log_level = level;
}

/**
 * Ensure that we're initialised at least once.
 * nica may have been statically linked into the final binary/library
 * consumer of the log APIs, which means our constructors might not
 * actually trigger.
 */
static void nc_log_maybe_init(void)
{
        static bool nc_log_had_init = false;
        if (nc_log_had_init) {
                return;
        }
        nc_log_had_init = true;
        nc_log_init(stderr);
}

/**
 * Ensure we're always at least initialised with stderr
 */
__attribute__((constructor)) static void nc_log_first_init(void)
{
        nc_log_maybe_init();
}

static inline const char *nc_log_level_str(NcLogLevel l)
{
        if (l <= NC_LOG_LEVEL_FATAL) {
                return log_str_table[l];
        }
        return "unknown";
}

static inline void nc_log_throw_format_error(FILE *stream)
{
        fputs("[FATAL]", stream);
        fputs(" Cannot log to stream: ", stream);
        fputs(strerror(errno), stream);
        fputs("\n", stream);
}

void nc_log(NcLogLevel level, const char *filename, int lineno, const char *format, ...)
{
        const char *displ = NULL;
        va_list vargs;
        autofree(char) *rend = NULL;
        nc_log_maybe_init();
        int r = 0;
        bool show_lines;

        /* Respect minimum log level */
        if (level < min_log_level) {
                return;
        }

        /* Only show lines when there is a debug or general problem */
        show_lines = (level == NC_LOG_LEVEL_DEBUG || level >= NC_LOG_LEVEL_ERROR);

        displ = nc_log_level_str(level);

        va_start(vargs, format);

        if (vasprintf(&rend, format, vargs) < 0) {
                nc_log_throw_format_error(log_file);
                goto clean_args;
        }

        if (show_lines) {
                r = fprintf(log_file, "[%s] (%s:L%d): %s\n", displ, filename, lineno, rend);
        } else {
                r = fprintf(log_file, "[%s]: %s\n", displ, rend);
        }

        if (r < 0) {
                /* Forcibly fall back to stderr with the format error msg */
                nc_log_throw_format_error(stderr);
        }

clean_args:
        va_end(vargs);
}

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
