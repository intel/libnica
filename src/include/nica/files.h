/*
 * This file is part of libnica.
 *
 * Copyright (C) 2016 Intel Corporation
 *
 * libnica is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 */

#pragma once

#ifndef LIBNICA_INTERNAL
#error You should only include <nica.h>
#endif

#include <nica/macros.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdarg.h>

/**
 * Determine whether the given path exists in the filesystem
 *
 * @param path Path to the file we should check
 */
_nica_public_ bool nc_file_exists(const char *path);

/**
 * Return the parent path for a given file
 *
 * @note This is an allocated string, and must be freed by the caller
 * @param p Path to file
 * @return a newly allocated string
 */
_nica_public_ char *nc_get_file_parent(const char *path);

/**
 * Recursively make the directories for the given path
 *
 * @param path Path to directory to create
 * @param mode Mode to create new directories with
 *
 * @return A boolean value, indicating success or failure
 */
_nica_public_ bool nc_mkdir_p(const char *path, mode_t mode);

/**
 * Recursively remove the tree at @path
 *
 * @param path The path to remove (and all children)
 *
 * @return A boolean value, indicating success or failure
 */
_nica_public_ bool nc_rm_rf(const char *path);

/**
 * Simple utility to copy path @src to path @dst, with mode @mode
 *
 * @note This will truncate the target if it exists, and does
 * not preserve stat information, when @remove_target is set to true
 *
 * @param src Path to the source file
 * @param dst Path to the destination file
 * @param mode Mode of the new file when creating
 * @param remove_target Whether to remove an existing target file first
 *
 * @return True if this succeeded
 */
_nica_public_ bool nc_copy_file(const char *src, const char *dst, mode_t mode,
                                bool remove_target);

/**
 * Gradually build up a valid path, which may point within an existing
 * tree, to mitigate any case sensitivity issues on case-preserving
 * filesystems
 */
_nica_public_ __attribute__((sentinel(0))) char *nc_build_case_correct_path(
    const char *c, ...);

_nica_public_ char *nc_build_case_correct_path_va(const char *c, va_list ap);

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
