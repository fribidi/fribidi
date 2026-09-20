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

if CROSS_COMPILING
# help2man needs to execute the freshly built binary to query its --help
# output, which is not possible when that binary is built for a different
# host. Fall back to a man page shipped in the source tree (e.g. from a
# release tarball) if there is one, otherwise skip generating it.
%.1: %
	@if test -f $(srcdir)/$@; then \
	  cp $(srcdir)/$@ $@; \
	else \
	  echo "$@: skipping man page generation; help2man can't run $< while cross-compiling" >&2; \
	  : > $@; \
	fi
else
%.1: %
	$(top_srcdir)/missing --run help2man ./$< --no-info \
	--version-string=- -n "$(shell ./$< --help | head -n 2 | tail -n 1)" \
	--output=$@
endif

-include $(top_srcdir)/git.mk
