bin_PROGRAMS = fribidi

noinst_PROGRAMS = fribidi-benchmark bidi-type-serialize

getopt_SOURCES = getopt.c getopt1.c getopt_int.h getopt.h

fribidi_SOURCES = fribidi-main.c $(getopt_SOURCES)

fribidi_benchmark_SOURCES = fribidi-benchmark.c $(getopt_SOURCES)

AM_CPPFLAGS = \
		-I$(top_builddir)/lib \
		-I$(top_srcdir)/lib \
		-I$(top_srcdir)/charset \
		$(MISC_CFLAGS)

LDADD = $(top_builddir)/lib/libfribidi.la $(GLIB_LIBS)

if FRIBIDI_CHARSETS
else # !FRIBIDI_CHARSETS
LDADD += $(top_builddir)/charset/libfribidi-char-sets.la
endif # !FRIBIDI_CHARSETS

$(top_builddir)/lib/libfribidi.la:
	cd $(top_builddir)/lib && $(MAKE) $(AM_MAKEFLAGS) libfribidi.la
