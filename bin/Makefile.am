bin_PROGRAMS = fribidi

noinst_PROGRAMS = fribidi-benchmark fribidi-bidi-types fribidi-caprtl2utf8

getopt_SOURCES = getopt.c getopt1.c getopt_int.h getopt.h gettext.h

fribidi_SOURCES = fribidi-main.c $(getopt_SOURCES)

fribidi_benchmark_SOURCES = fribidi-benchmark.c $(getopt_SOURCES)

AM_CPPFLAGS = \
		@FRIBIDI_CPPFLAGS@ \
		-I$(top_builddir)/lib \
		-I$(top_srcdir)/lib

LDADD = $(top_builddir)/lib/libfribidi.la

dist_man_MANS = $(bin_PROGRAMS:=.1)
MAINTAINERCLEANFILES = $(man1)

man1 = $(dist_man_MANS)

$(top_builddir)/lib/libfribidi.la:
	cd $(top_builddir)/lib && $(MAKE) $(AM_MAKEFLAGS) libfribidi.la

%.1: %
	$(top_srcdir)/missing --run help2man ./$< --no-info \
	--version-string=- -n "$(shell ./$< --help | head -n 2 | tail -n 1)" \
	--output=$@

-include $(top_srcdir)/git.mk
