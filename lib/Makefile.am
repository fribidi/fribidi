lib_LTLIBRARIES = libfribidi.la

AM_CPPFLAGS = $(MISC_CFLAGS)

libfribidi_la_LDFLAGS = -version-info $(LT_VERSION_INFO)
libfribidi_la_LIBADD = $(MISC_LIBS)
libfribidi_la_DEPENDENCIES =

if OS_WIN32
libfribidi_la_LDFLAGS += -export-symbols $(srcdir)/fribidi.def
endif # OS_WIN32

if PLATFORM_WIN32
libfribidi_la_LDFLAGS += -no-undefined
endif # PLATFORM_WIN32

if FRIBIDI_CHARSETS

AM_CPPFLAGS += -I$(top_srcdir)/charset
libfribidi_la_LIBADD += $(top_builddir)/charset/libfribidi-char-sets.la
libfribidi_la_DEPENDENCIES += $(top_builddir)/charset/libfribidi-char-sets.la

$(top_builddir)/charset/libfribidi-char-sets.la:
	cd $(top_builddir)/lib && $(MAKE) $(AM_MAKEFLAGS) libfribidi-char-sets.la

endif # FRIBIDI_CHARSETS

include Headers.mk
pkginclude_HEADERS = $(libfribidi_la_headers)

libfribidi_la_SOURCES =	\
		bidi-types.h \
		bidi-types-list.h \
		bidi-type.tab.i \
		common.h \
		debug.h \
		env.h \
		fribidi.c \
		fribidi-bidi.c \
		fribidi-bidi-type.c \
		fribidi-bidi-types.c \
		fribidi-env.c \
		fribidi-mem.c \
		fribidi-mirroring.c \
		fribidi-run.c \
		mem.h \
		mirroring.tab.i \
		run.h

BUILT_SOURCES= \
		bidi-type.tab.i \
		mirroring.tab.i \
		fribidi-unicode-version.h

$(BUILT_SOURCES):
	(cd $(top_builddir)/gen.tab && \
	 $(MAKE) $(AM_MAKEFLAGS) $@) && \
	mv $(top_builddir)/gen.tab/$@ .

MAINTAINERCLEANFILES = $(BUILT_SOURCES)

$(top_builddir)/config.h: $(top_srcdir)/config.h.in
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) config.h
