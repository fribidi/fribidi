lib_LTLIBRARIES = libfribidi.la

AM_CPPFLAGS = $(GLIB_CFLAGS)

libfribidi_la_LDFLAGS = -version-info $(LT_VERSION_INFO)
libfribidi_la_LIBADD = $(GLIB_LIBS)
libfribidi_la_DEPENDENCIES =

if PLATFORM_WIN32
libfribidi_la_LDFLAGS += -no-undefined
endif # PLATFORM_WIN32

if OS_WIN32
libfribidi_la_LDFLAGS += -export-symbols $(srcdir)/fribidi.def
endif # OS_WIN32

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
		fribidi-unicode-version.h \
		fribidi-config.h

MAINTAINERCLEANFILES = $(BUILT_SOURCES)

fribidi_tab_mirroring_0.i: $(fribidi_create_mirroring_SOURCES) fribidi_types.h	\
		unidata/BidiMirroring.txt
	$(MAKE) fribidi_create_mirroring
	if test -d unidata; then UNIDATA=unidata;	\
	else UNIDATA="$(srcdir)/unidata"; fi &&	\
	./fribidi_create_mirroring "$$UNIDATA" ||	\
	($(RM) "$@"; false)

fribidi_tab_bidi_type.i: $(fribidi_create_char_types_SOURCES)	\
		unidata/UnicodeData.txt
	$(MAKE) fribidi_create_char_types
	if test -d unidata; then UNIDATA=unidata;	\
	else UNIDATA="$(srcdir)/unidata"; fi &&	\
	./fribidi_create_char_types "`echo "$@"	\
	  | $(SED) 's/[^0-9]*//g'`" "$$UNIDATA" ||	\
	($(RM) "$@"; false)
	$(RM) fribidi_tab_char_type_stamp
	$(MAKE) fribidi_tab_char_type_stamp

$(top_builddir)/config.h: $(top_srcdir)/config.h.in
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) config.h
