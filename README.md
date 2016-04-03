libnica
-------

[![Build Status](https://travis-ci.org/ikeydoherty/libnica.svg?branch=master)](https://travis-ci.org/ikeydoherty/libnica)
[![Coverage Status](https://coveralls.io/repos/github/ikeydoherty/libnica/badge.png?branch=master)](https://coveralls.io/github/ikeydoherty/libnica?branch=master)

*Common C library functions*

Lightweight library containing common functionality of typical "C utility libs".

Currently contains:

 - Dynamic array
 - List type
 - Simplified file manipulation functions (mkdir_p, rm_rf, etc)
 - A Hashmap implementation
 - A simple, efficient, INI File parser
 - Safe string manipulation functions for when strlen is out of the question

The majority of the API is designed to work on a RAII principle, using scope-based
`autofree` helpers (see util.h) with minimal fuss.

Another important principle to keep in mind while hacking on `libnica`, is that
memory leaks of any description in a successful usage, are forbidden. This includes
any "possibly lost" memory through static initialisation as seen with heavier
libraries.


License
------

LGPL-2.1

Copyright (C) 2014-2016 Intel Corporation
