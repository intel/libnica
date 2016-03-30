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

/**
 * An INI-format Configuration File
 */
typedef struct NcIniFile {
        int __reserved1;
} NcIniFile;


NcIniFile *nc_ini_file_parse(const char *path);

void nc_ini_file_free(NcIniFile *file);

DEF_AUTOFREE(NcIniFile, nc_ini_file_free)

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
