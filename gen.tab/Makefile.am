EXTRA_PROGRAMS = \
		gen-bidi-type-tab \
		gen-mirroring-tab \
		gen-unicode-version

gen_bidi_type_tab_SOURCES = gen-bidi-type-tab.c packtab.c packtab.h
gen_mirroring_tab_SOURCES = gen-mirroring-tab.c
gen_unicode_version_SOURCES = gen-unicode-version.c

CLEANFILES = $(EXTRA_PROGRAMS)

AM_CPPFLAGS = \
		-I$(top_builddir)/lib \
		-I$(top_srcdir)/lib \
		-I$(top_srcdir)/charset \
		$(MISC_CFLAGS)

VPATH += \
		$(builddir)/unidata \
		$(builddir)/unidata/extracted \
		$(srcdir)/unidata \
		$(srcdir)/unidata/extracted \
		$(top_builddir)/lib

# bidi-type.tab.i

COMPRESSION = 4

GEN_BIDI_TYPE_TAB = \
	$(MAKE) $(AM_MAKEFLAGS) gen-bidi-type-tab && \
	(DATA_FILE_TYPE=`echo $< | sed s,.*/,,`; \
	./gen-bidi-type-tab $(COMPRESSION) \
	 $$DATA_FILE_TYPE $< > $@ || ($(RM) $@ && false))

bidi-type_UnicodeData.tab.i: \
		UnicodeData.txt \
		fribidi-unicode-version.h \
		$(gen_bidi_type_tab_sources)

bidi-type_DerivedBidiClass.tab.i: \
		DerivedBidiClass.txt \
		fribidi-unicode-version.h \
		$(gen_bidi_type_tab_sources)

bidi-type_UnicodeData.tab.i bidi-type_DerivedBidiClass.tab.i:
	$(GEN_BIDI_TYPE_TAB)

bidi-type.tab.i:
	@ \
	($(MAKE) $(AM_MAKEFLAGS) bidi-type_DerivedBidiClass.tab.i \
	&& mv bidi-type_DerivedBidiClass.tab.i $@) \
	|| \
	($(MAKE) $(AM_MAKEFLAGS) bidi-type_UnicodeData.tab.i \
	&& mv bidi-type_UnicodeData.tab.i $@) \
	|| \
	(echo Cannot build $@. \
	&& false)

BUILT_TEMP_FILES = \
		bidi-type_DerivedBidiClass.tab.i \
		bidi-type_UnicodeData.tab.i
