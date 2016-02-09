ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = ${top_srcdir}/README.md \
	     ${top_srcdir}/LICENSE.LGPL2.1 \
	     ${top_srcdir}/HACKING \
	     ${top_srcdir}/findstatic.pl \
	     ${top_srcdir}/data/libthingamabob.pc.in

NULL =
CLEANFILES =

AM_CFLAGS = \
	-fstack-protector -Wall -pedantic \
	-Wstrict-prototypes -Wundef -fno-common \
	-Werror-implicit-function-declaration \
	-Wformat -Wformat-security -Werror=format-security \
	-Wno-conversion -Wunused-variable -Wunreachable-code \
	-Wall -W -D_FORTIFY_SOURCE=2 -std=c99 -Werror \
	-DLIBTHINGAMABOB_INTERNAL

AM_CPPFLAGS = \
	-I $(top_srcdir)/src \
	-I $(top_srcdir)/src/include

AUTOMAKE_OPTIONS = color-tests parallel-tests

if COVERAGE
coverage:
	mkdir -p coverage
	lcov --compat-libtool --directory . --capture --output-file coverage/report
	genhtml -o coverage/ coverage/report
AM_CFLAGS += --coverage
endif


lib_LTLIBRARIES = libthingamabob.la

# Our main headers
thingamabobinst_HEADERS = \
	src/include/array.h \
	src/include/hashmap.h \
	src/include/util.h

# The main one you should include
include_HEADERS = \
	src/include/thingamabob.h

# Pkgconfig file
pcfiledir = $(libdir)/pkgconfig
pcfile_DATA = \
	data/libthingamabob.pc

thingamabobinstdir = $(includedir)/libthingamabob

# Main library
libthingamabob_la_SOURCES = \
	src/array.c \
	src/hashmap.c \
	src/util.c

libthingamabob_la_CFLAGS = \
	$(AM_CFLAGS)

# Tests
TESTS = \
	check_core \
	check_array \
	check_hashmap

check_PROGRAMS = $(TESTS)

# Core tests
check_core_SOURCES = \
	tests/check-core.c

check_core_CFLAGS = \
	$(CHECK_CFLAGS) \
	$(AM_CFLAGS)

check_core_LDADD = \
	$(CHECK_LIBS)

# Test array
check_array_SOURCES = \
	tests/check-array.c

check_array_CFLAGS = \
	$(CHECK_CFLAGS) \
	$(AM_CFLAGS)

check_array_LDADD = \
	$(CHECK_LIBS)

# Test hashmap
check_hashmap_SOURCES = \
	tests/check-hashmap.c

check_hashmap_CFLAGS = \
	$(CHECK_CFLAGS) \
	$(AM_CFLAGS)

check_hashmap_LDADD = \
	$(CHECK_LIBS)

@VALGRIND_CHECK_RULES@
