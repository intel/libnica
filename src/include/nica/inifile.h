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

#include <nica/util.h>
#include <nica/hashmap.h>

typedef enum {
        NC_INI_ERROR_MIN = 0,
        NC_INI_ERROR_FILE,
        NC_INI_ERROR_EMPTY_KEY,
        NC_INI_ERROR_NOT_CLOSED,
        NC_INI_ERROR_NO_SECTION,
        NC_INI_ERROR_INVALID_LINE,
        NC_INI_ERROR_MAX
} NcIniError;

NcHashmap *nc_ini_file_parse(const char *path);
int nc_ini_file_parse_full(const char *path, NcHashmap **out_map, int *error_line_number);
const char *nc_ini_error(NcIniError error);

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
