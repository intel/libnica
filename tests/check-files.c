/*
 * This file is part of libnica.
 *
 * Copyright © 2016-2018 Intel Corporation
 *
 * libnica is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 */

#define _GNU_SOURCE
#include <check.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "util.h"

#include "files.c"

START_TEST(nc_file_exists_test)
{
        char *path = "nc-file-exist-test-file";
        fail_if(open(path, O_CREAT, S_IRWXU) == -1, "Unable to create test file");
        fail_if(!nc_file_exists(path), "Failed to stat existing file");
        fail_if(unlink(path), "Failed to remove file");
        fail_if(nc_file_exists(path), "Erroneously detected non-existent file");
}
END_TEST

START_TEST(nc_get_file_parent_test)
{
        char *path = "nc-parent-test-file";
        char *real_parent = get_current_dir_name();
        fail_if(!real_parent, "Unable to get current directory path");
        char *parent;
        fail_if(open(path, O_CREAT, S_IRWXU) == -1, "Unable to create test file");
        parent = nc_get_file_parent(path);
        fail_if(strcmp(real_parent, parent), "parent folder incorrect: %s vs %s", real_parent, parent);
        free(parent);
        free(real_parent);
        fail_if(unlink(path), "Failed to remove file");
}
END_TEST

#define RM_RF_BASE_PATH "nc-rm-test-dir/"

START_TEST(nc_rm_rf_test)
{
        struct stat sb;
        char *sub_dir = RM_RF_BASE_PATH "nc-rm-test-sub-dir/";
        char *sub_file = RM_RF_BASE_PATH "nc-rm-test-sub-file";
        char *good_sym_file = RM_RF_BASE_PATH "nc-rm-test-good-sym-file";
        char *bad_sym_file = RM_RF_BASE_PATH "nc-rm-test-bad-sym-file";
        char *good_sym_dir = RM_RF_BASE_PATH "nc-rm-test-good-sym-dir";
        char *good_sym_pdir = RM_RF_BASE_PATH "nc-rm-test-good-sym-pdir";;
        char *bad_sym_dir = RM_RF_BASE_PATH "nc-rm-test-bad-sym-dir";
        fail_if(mkdir(RM_RF_BASE_PATH, S_IRWXU), "Unable to create test directory");
        fail_if(mkdir(sub_dir, S_IRWXU), "Unable to create test subdirectory");
        fail_if(open(sub_file, O_CREAT, S_IRWXU) == -1, "Unable to create test file");
        fail_if(symlink(sub_file, good_sym_file), "Unable to create good file symlink");
        fail_if(symlink(sub_dir, good_sym_dir), "Unable to create good dir symlink");
        fail_if(symlink(".", good_sym_pdir), "Unable to create good pdir symlink");
        fail_if(symlink("does-not-exist-dir", bad_sym_dir), "Unable to create bad dir symlink");
        fail_if(symlink("does-not-exist-file", bad_sym_file), "Unable to create bad file symlink");
        fail_if(!nc_rm_rf(RM_RF_BASE_PATH), "Unable to remove directory tree");
        fail_if(!stat(RM_RF_BASE_PATH, &sb) && errno != ENOENT, "Failed to validate directory tree removal");
}
END_TEST

static Suite *core_suite(void)
{
        Suite *s = NULL;
        TCase *tc = NULL;

        s = suite_create("nc_files");
        tc = tcase_create("nc_file_functions");
        tcase_add_test(tc, nc_file_exists_test);
        tcase_add_test(tc, nc_get_file_parent_test);
        tcase_add_test(tc, nc_rm_rf_test);
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
