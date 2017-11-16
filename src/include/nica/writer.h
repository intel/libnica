/*
 * This file is part of libnica.
 *
 * Copyright © 2017 Intel Corporation
 *
 * libnica is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 */

#pragma once

#include <nica/util.h>

#define _GNU_SOURCE

typedef struct NcWriter {
        FILE *memstream;
        char *buffer;
        size_t buffer_n;
        int error;
} NcWriter;

#define NC_WRITER_INIT &(NcWriter){ 0 };

/**
 * Construct a new NcWriter
 */
bool nc_writer_open(NcWriter *writer);

/**
 * Clean up a previously allocated NcWriter
 */
void nc_writer_free(NcWriter *writer);

/**
 * Close the writer, which will ensure that the buffer is NULL terminated.
 * No more writes are possible after this close.
 */
void nc_writer_close(NcWriter *writer);

/**
 * Append string to the buffer
 */
void nc_writer_append(NcWriter *writer, const char *s);

/**
 * Append, printf style, to the buffer
 */
void nc_writer_append_printf(NcWriter *writer, const char *fmt, ...)
    __attribute__((format(printf, 2, 3)));

/**
 * Return an error that may exist in the stream, otherwise 0.
 * This allows utilising NcWriter in a failsafe fashion, and checking the
 * error once only.
 */
int nc_writer_error(NcWriter *writer);

/* Convenience: Automatically clean up the NcWriter */
DEF_AUTOFREE(NcWriter, nc_writer_free)

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
