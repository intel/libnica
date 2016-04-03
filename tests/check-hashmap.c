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

#define _GNU_SOURCE
#include <check.h>
#include <stdlib.h>
#include "hashmap.c"

START_TEST(nc_hashmap_new_check)
{
        NcHashmap *map = NULL;

        map = nc_hashmap_new(NULL, NULL);
        fail_if(!map, "Failed to allocate new hashmap");
        fail_if(nc_hashmap_size(map) != 0, "Hashmap size is not 0 after new");
        nc_hashmap_free(map);
}
END_TEST


static Suite *nc_hashmap_suite(void)
{
        Suite *s = NULL;
        TCase *tc = NULL;

        s = suite_create("nc_hashmap");
        tc = tcase_create("nc_hashmap_functions");
        tcase_add_test(tc, nc_hashmap_new_check);
        suite_add_tcase(s, tc);

        return s;
}

int main(void)
{
        int number_failed;
        Suite *s;
        SRunner *sr;

        s = nc_hashmap_suite();
        sr = srunner_create(s);
        srunner_run_all(sr, CK_VERBOSE);
        number_failed = srunner_ntests_failed(sr);
        srunner_free(sr);

        return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
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
