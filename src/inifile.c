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

#include <stdlib.h>

#include "inifile.h"

NcIniFile *nc_ini_file_parse(__attribute__ ((unused)) const char *path)
{
        NcIniFile *ret = NULL;

        ret = calloc(1, sizeof(struct NcIniFile));
        if (!ret) {
                return NULL;
        }

        return ret;
}

void nc_ini_file_free(NcIniFile *file)
{
        if (!file) {
                return;
        }
        free(file);
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
