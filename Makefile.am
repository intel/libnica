ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = ${srcdir}/README.md \
	     ${srcdir}/LICENSE.LGPL2.1 \
	     ${srcdir}/HACKING \
	     ${srcdir}/findstatic.pl \
	     ${srcdir}/data/nica.pc.in \
	     ${srcdir}/tests/ini/wellformed.ini \
	     ${srcdir}/tests/ini/valid_padding.ini \
	     ${srcdir}/tests/ini/sectionless.ini \
	     ${srcdir}/tests/ini/empty_key.ini \
	     ${srcdir}/tests/ini/just_assign.ini \
	     ${srcdir}/tests/ini/broken_section_start.ini \
	     ${srcdir}/tests/ini/broken_section_end.ini \
	     ${srcdir}/current_symbols \
	     ${srcdir}/abi.sh

NULL =
CLEANFILES =

# set library version info
LIBNICA_CURRENT=0
LIBNICA_REVISION=2
LIBNICA_AGE=0

AM_CFLAGS = \
	-fstack-protector -Wall -pedantic \
	-Wstrict-prototypes -Wundef -fno-common \
	-Werror-implicit-function-declaration \
	-Wformat -Wformat-security -Werror=format-security \
	-Wconversion -Wunused-variable -Wunreachable-code \
	-Wall -W -D_FORTIFY_SOURCE=2 -std=c11

AM_CPPFLAGS = \
	-I $(srcdir)/src \
	-I $(srcdir)/src/include \
	-I $(srcdir)/src/include/nica \
	-DTOP_DIR=\"$(srcdir)\"

AUTOMAKE_OPTIONS = color-tests parallel-tests

NICA_SOURCE_FILES = \
	src/array.c \
	src/hashmap.c \
	src/files.c \
	src/inifile.c \
	src/list.c \
	src/log.c \
	src/nc-string.c \
	src/util.c \
	src/writer.c


# Allows integrating nica as a submodule..
if NICA_SUBMODULE

noinst_LTLIBRARIES = libnica.la

libnica_la_SOURCES = $(NICA_SOURCE_FILES)
libnica_la_CFLAGS = $(AM_CFLAGS)
libnica_la_LDFLAGS = $(AM_LDFLAGS) -export-dynamic

else

# Standard build, as a shared library

if COVERAGE
coverage:
	mkdir -p coverage
	lcov --compat-libtool --directory . --capture --output-file coverage/report
	lcov --remove coverage/report 'tests/*' '/usr/*' --output-file coverage/report
	genhtml -o coverage/ coverage/report

AM_CFLAGS += --coverage
endif

install-exec-hook:
	eval bash "$(srcdir)/abi.sh" "$(srcdir)" "$(builddir)"

lib_LTLIBRARIES = libnica.la

# Our main headers
nicainst_HEADERS = \
	src/include/nica/array.h \
	src/include/nica/hashmap.h \
	src/include/nica/files.h \
	src/include/nica/macros.h \
	src/include/nica/inifile.h \
	src/include/nica/list.h \
	src/include/nica/log.h \
	src/include/nica/nc-string.h \
	src/include/nica/util.h \
	src/include/nica/writer.h

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
	$(NICA_SOURCE_FILES)

libnica_la_CFLAGS = \
	$(AM_CFLAGS) \
	-fvisibility=hidden

libnica_la_LDFLAGS = \
	$(AM_LDFLAGS) \
	-version-info $(LIBNICA_CURRENT):$(LIBNICA_REVISION):$(LIBNICA_AGE) \
	-export-dynamic

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

endif
