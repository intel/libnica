#!/bin/bash
#
# This file is part of libnica.
#
# Copyright (C) 2016 Intel Corporation
#
# libnica is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation; either version 2.1
# of the License, or (at your option) any later version.
# 

function check_abi()
{
    whence="$1"
    nm -g $1 |grep " T "|awk '{print $3}'|grep -v "^_init" |grep -v "^_fini" |sort
}

if [[ ! -z "${1}" ]]; then
    check_abi "${2}"/.libs/*.so.*.* > newsyms
    diff -ruNb "${1}"/current_symbols newsyms
else
    check_abi .libs/*.so.*.* > newsyms
    diff -ruNb current_symbols newsyms
fi

if [[ $? -eq 0 ]]; then
        rm newsyms
        exit 0
fi

echo "ABI change detected"
exit 1
