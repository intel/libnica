#!/bin/bash
clang-format -i $(find . -name '*.[ch]')

# Look for typos
misspell `find . -name "*.c"  -or -name "*.h"` || exit 1
