ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = ${top_srcdir}/README.md \
	     ${top_srcdir}/LICENSE.LGPL2.1 \
	     ${top_srcdir}/HACKING \
	     ${top_srcdir}/findstatic.pl

NULL =
CLEANFILES =

AM_CFLAGS = \
	-fstack-protector -Wall -pedantic \
	-Wstrict-prototypes -Wundef -fno-common \
	-Werror-implicit-function-declaration \
	-Wformat -Wformat-security -Werror=format-security \
	-Wno-conversion -Wunused-variable -Wunreachable-code \
	-Wall -W -D_FORTIFY_SOURCE=2 -std=c99 -Werror

AM_CPPFLAGS = \
	-I $(top_srcdir)/src

AUTOMAKE_OPTIONS = color-tests parallel-tests

if COVERAGE
coverage:
	mkdir -p coverage
	lcov --compat-libtool --directory . --capture --output-file coverage/report
	genhtml -o coverage/ coverage/report
AM_CFLAGS += --coverage
endif


lib_LTLIBRARIES = libthingamabob.la

libthingamabob_la_SOURCES = \
	src/util.c \
	src/util.h

libthingamabob_la_CFLAGS = \
	$(AM_CFLAGS)
