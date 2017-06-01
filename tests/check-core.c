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
#include <check.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

#include "log.c"
#include "nc-string.c"
#include "writer.c"

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

START_TEST(nc_memory_test)
{
        {
                autofree(memtestchar) *tmp = NULL;

                if (asprintf(&tmp, "Allocation test") < 0) {
                        fail("Unable to allocate memory");
                }
        }
        fail_if(!reclaimed, "Scope based tmp var was not reclaimed!");
}
END_TEST

START_TEST(nc_string_test)
{
        nc_string *str = NULL, *str2 = NULL;
        char *c = NULL;

        str = nc_string_dup_printf("Test String #%d", 1);
        fail_if(!str, "Failed to allocate string");
        fail_if(!streq(str->str, "Test String #1"), "Invalid formatted string");
        fail_if(ncstrlen(str) != 14, "Incorrect string length");

        fail_if(!nc_string_has_suffix(str, "g #1"), "String has incorrect suffix");
        fail_if(!nc_string_has_prefix(str, "T"), "String has incorrect prefix");

        fail_if(!nc_string_cat(str, "append"), "Failed to append string");
        fail_if(ncstrlen(str) != 20, "Incorrect string length after append");

        str2 = nc_string_dup(str->str);
        fail_if(!streq(str->str, str2->str), "Strings do not match");
        fail_if(ncstrlen(str) != ncstrlen(str2), "Invalid string length after dup");

        nc_string_free(str);
        nc_string_free(str2);

        str = nc_string_dup("  spacey text  ");
        c = nc_string_strip(str);
        fail_if(!c, "Failed to strip string");
        fail_if(!streq(c, "spacey text"), "Failed to match stripped string");
        nc_string_free(str);

        str = nc_string_dup_printf("Test String #%d", 2);
        fail_if(!str, "Failed to allocate string");
        fail_if(!nc_string_const_equal(str, "Test String #2"), "Const String Compare failed");
        /* Ensure its not failing completely. */
        fail_if(nc_string_const_equal(str, "Test String #1"),
                "Const String Compare should not match");

        str2 = nc_string_dup(str->str);
        fail_if(!str2, "Failed to dup string");
        fail_if(!nc_string_equal(str, str2), "Identical strings not matching");

        fail_if(!nc_string_const_equal(str2, (const char *)str->str), "Direct const compare fail!");

        nc_string_free(str);
        nc_string_free(str2);

        fail_if(nc_string_dup(NULL), "dup on NULL string");
        fail_if(nc_string_dup_printf(NULL), "dup_printf on NULL string");
        fail_if(nc_string_cat(NULL, NULL), "cat on NULL string");

        fail_if(nc_string_equal(NULL, NULL), "equal on NULL string");
        fail_if(nc_string_const_equal(NULL, NULL), "const_equal on NULL string");

        /* Forced empty ->str tests */
        nc_string st = {.len = 0 };
        fail_if(nc_string_equal(&st, &st), "equal on NULL ->str");
        fail_if(nc_string_const_equal(&st, "TEST"), "const_equal on NULL ->str");
}
END_TEST

START_TEST(nc_writer_test)
{
        autofree(NcWriter) *writer = NC_WRITER_INIT;
        const char *exp = "This is string one\nThis is: string two\n";

        fail_if(!nc_writer_open(writer), "Failed to open NcWriter for writing");

        nc_writer_append(writer, "This is string one\n");
        nc_writer_append_printf(writer, "This is: %s\n", "string two");

        fail_if(nc_writer_error(writer) != 0, "Writer should not have an error set");
        nc_writer_close(writer);

        fail_if(!streq(writer->buffer, exp), "Buffer does not match expected contents");

        /* Should definitely fail to write to a closed buffer */
        nc_writer_append(writer, "Another string: one");
        nc_writer_append_printf(writer, "Another string: %s", "two");

        fail_if(nc_writer_error(writer) != EBADF, "Writer did not close when requested");
        fail_if(!streq(writer->buffer, exp), "Buffer was modified after close");
}
END_TEST

/**
 * This is a very trivial test, which will basically either segfault or not.
 */
START_TEST(nc_log_test)
{
        setenv("NC_DEBUG", "1", 1);
        nc_log_init(NULL);
        NC_LOG_DEBUG("Starting tests");
        NC_LOG_INFO("Info message");
        NC_LOG_ERROR("Error message");
        NC_LOG_FATAL("Fatal message");
        NC_LOG_SUCCESS("Success message");
        unsetenv("NC_DEBUG");
        nc_log_init(NULL);
        NC_LOG_DEBUG("This should never be seen");
}
END_TEST

static Suite *core_suite(void)
{
        Suite *s = NULL;
        TCase *tc = NULL;

        s = suite_create("nc_core");
        tc = tcase_create("nc_core_functions");
        tcase_add_test(tc, nc_memory_test);
        tcase_add_test(tc, nc_string_test);
        tcase_add_test(tc, nc_writer_test);
        tcase_add_test(tc, nc_log_test);
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
