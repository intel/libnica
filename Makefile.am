ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = ${top_srcdir}/README.md \
	     ${top_srcdir}/LICENSE.LGPL2.1 \
	     ${top_srcdir}/HACKING \
	     ${top_srcdir}/findstatic.pl \
	     ${top_srcdir}/data/nica.pc.in \
	     ${top_srcdir}/tests/ini/wellformed.ini \
	     ${top_srcdir}/tests/ini/valid_padding.ini \
	     ${top_srcdir}/tests/ini/sectionless.ini \
	     ${top_srcdir}/tests/ini/empty_key.ini \
	     ${top_srcdir}/tests/ini/just_assign.ini \
	     ${top_srcdir}/tests/ini/broken_section_start.ini \
	     ${top_srcdir}/tests/ini/broken_section_end.ini

NULL =
CLEANFILES =

# set library version info
LIBNICA_CURRENT=1
LIBNICA_REVISION=0
LIBNICA_AGE=0

AM_CFLAGS = \
	-fstack-protector -Wall -pedantic \
	-Wstrict-prototypes -Wundef -fno-common \
	-Werror-implicit-function-declaration \
	-Wformat -Wformat-security -Werror=format-security \
	-Wno-conversion -Wunused-variable -Wunreachable-code \
	-Wall -W -D_FORTIFY_SOURCE=2 -std=c99 -Werror \
	-DLIBNICA_INTERNAL

AM_CPPFLAGS = \
	-I $(top_srcdir)/src \
	-I $(top_srcdir)/src/include \
	-I $(top_srcdir)/src/include/nica \
	-DTOP_DIR=\"$(abs_top_srcdir)\"

AUTOMAKE_OPTIONS = color-tests parallel-tests

if COVERAGE
coverage:
	mkdir -p coverage
	lcov --compat-libtool --directory . --capture --output-file coverage/report
	genhtml -o coverage/ coverage/report
AM_CFLAGS += --coverage
endif


lib_LTLIBRARIES = libnica.la

# Our main headers
nicainst_HEADERS = \
	src/include/nica/array.h \
	src/include/nica/hashmap.h \
	src/include/nica/files.h \
	src/include/nica/inifile.h \
	src/include/nica/list.h \
	src/include/nica/nc-string.h \
	src/include/nica/util.h

# The main one you should include
include_HEADERS = \
	src/include/nica/nica.h

# Pkgconfig file
pcfiledir = $(libdir)/pkgconfig
pcfile_DATA = \
	data/nica.pc

nicainstdir = $(includedir)/nica

# Main library
libnica_la_SOURCES = \
	src/array.c \
	src/hashmap.c \
	src/files.c \
	src/inifile.c \
	src/list.c \
	src/nc-string.c \
	src/util.c

libnica_la_CFLAGS = \
	$(AM_CFLAGS)

libnica_la_LDFLAGS = \
	$(AM_LDFLAGS) \
	-version-info $(LIBNICA_CURRENT):$(LIBNICA_REVISION):$(LIBNICA_AGE)

# Tests
TESTS = \
	check_core \
	check_array \
	check_hashmap \
	check_inifile \
	check_list

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

# Test inifile
check_inifile_SOURCES = \
	tests/check-inifile.c

check_inifile_CFLAGS = \
	$(CHECK_CFLAGS) \
	$(AM_CFLAGS)

check_inifile_LDADD = \
	$(CHECK_LIBS)

# Test list
check_list_SOURCES = \
	tests/check-list.c

check_list_CFLAGS = \
	$(CHECK_CFLAGS) \
	$(AM_CFLAGS)

check_list_LDADD = \
	$(CHECK_LIBS)

@VALGRIND_CHECK_RULES@
