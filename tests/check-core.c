/*
 * This file is part of libthingamabob.
 *
 * Copyright (C) 2016 Intel Corporation
 *
 * libthingamabob is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 */

#define _GNU_SOURCE
#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "util.h"

/**
 * Ensure scope based management is functional
 */
static bool reclaimed = false;
typedef char memtestchar;

static void reaper(void *v)
{
        free(v);
        v = NULL;
        fprintf(stderr, "Freeing tmp var\n");
        reclaimed = true;
}
DEF_AUTOFREE(memtestchar, reaper)

START_TEST(tb_memory_test)
{
        {
                autofree(memtestchar) *tmp = NULL;

                if (!asprintf(&tmp, "Allocation test")) {
                        fail("Unable to allocate memory");
                }
        }
        fail_if(!reclaimed, "Scope based tmp var was not reclaimed!");
}
END_TEST


static Suite *core_suite(void)
{
        Suite *s = NULL;
        TCase *tc = NULL;

        s = suite_create("tb_core");
        tc = tcase_create("tb_core_functions");
        tcase_add_test(tc, tb_memory_test);
        suite_add_tcase(s, tc);

        return s;
}

int main(void)
{
        Suite *s;
        SRunner *sr;
        int fail;

        s = core_suite();
        sr = srunner_create(s);
        srunner_run_all(sr, CK_VERBOSE);
        fail = srunner_ntests_failed(sr);
        srunner_free(sr);

        if (fail > 0) {
                return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
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
