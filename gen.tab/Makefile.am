noinst_PROGRAMS = \
		gen-bidi-type-tab \
		gen-mirroring-tab \
		gen-unicode-version

gen_bidi_type_tab_SOURCES = gen-bidi-type-tab.c packtab.c packtab.h
gen_mirroring_tab_SOURCES = gen-mirroring-tab.c
gen_unicode_version_SOURCES = gen-unicode-version.c

AM_CPPFLAGS = \
		-I$(top_builddir)/lib \
		-I$(top_srcdir)/lib \
		-I$(top_srcdir)/charset
