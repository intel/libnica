#!/bin/bash
set -e
clang-format -i $(find . -name '*.[ch]')

# Look for typos
misspell -error `find . -name "*.c"  -or -name "*.h"`
