noinst_LTLIBRARIES = libfribidi-char-sets.la

AM_CPPFLAGS = \
		-I$(top_builddir)/lib \
		-I$(top_srcdir)/lib \
		$(GLIB_CFLAGS)

pkginclude_HEADERS =

libfribidi_char_sets_la_LDFLAGS = 
libfribidi_char_sets_la_LIBADD = 

if FRIBIDI_CHARSETS

if PLATFORM_WIN32
no_undefined = -no-undefined
endif # PLATFORM_WIN32

libfribidi_char_sets_la_LDFLAGS += $(no_undefined)
libfribidi_char_sets_la_LIBADD += $(GLIB_LIBS)

if OS_WIN32
libfribidi_char_sets_la_LDFLAGS += -export-symbols
endif # OS_WIN32

include Headers.mk
pkginclude_HEADERS += $(libfribidi_char_sets_la_headers)

else # !FRIBIDI_CHARSETS

libfribidi_char_sets_la_LDFLAGS += -static

endif # !FRIBIDI_CHARSETS

libfribidi_char_sets_la_SOURCES = \
	fribidi-char-sets-cap-rtl.c \
	fribidi-char-sets-cp1255.c \
	fribidi-char-sets-cp1256.c \
	fribidi-char-sets.c \
	fribidi-char-sets-iso8859-6.c \
	fribidi-char-sets-iso8859-8.c \
	fribidi-char-sets-utf8.c

$(top_builddir)/config.h: $(top_srcdir)/config.h.in
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) config.h

$(top_builddir)/lib/fribidi-config.h: $(top_srcdir)/lib/fribidi-config.h.in
	cd $(top_builddir)/lib && $(MAKE) $(AM_MAKEFLAGS) fribidi-config.h
