EXTRA_PROGRAMS = \
		gen-bidi-type-tab \
		gen-mirroring-tab \
		gen-unicode-version

gen_bidi_type_tab_SOURCES = gen-bidi-type-tab.c packtab.c packtab.h
gen_mirroring_tab_SOURCES = gen-mirroring-tab.c
gen_unicode_version_SOURCES = gen-unicode-version.c

CLEANFILES = $(EXTRA_PROGRAMS)
DISTCLEANFILES =

AM_CPPFLAGS = \
		-I$(top_builddir)/lib \
		-I$(top_srcdir)/lib \
		-I$(top_srcdir)/charset \
		$(MISC_CFLAGS)

LDADD = $(MISC_LIBS)

VPATH += \
		$(builddir)/unidata \
		$(builddir)/unidata/extracted \
		$(srcdir)/unidata \
		$(srcdir)/unidata/extracted \
		$(top_builddir)/lib

# generate bidi-type.tab.i

gen_bidi_type_tab = gen-bidi-tab$(EXEEXT)

COMPRESSION = 4

BIDI_TYPE_TAB_VARIANTS = \
		UnicodeData_bidi-type.tab.i \
		DerivedBidiClass_bidi-type.tab.i

UnicodeData_bidi-type.tab.i: \
		UnicodeData.txt \
		fribidi-unicode-version.h \
		$(gen_bidi_type_tab_SOURCES)

DerivedBidiClass_bidi-type.tab.i: \
		DerivedBidiClass.txt \
		fribidi-unicode-version.h \
		$(gen_bidi_type_tab_SOURCES)

$(BIDI_TYPE_TAB_VARIANTS):
	$(MAKE) $(AM_MAKEFLAGS) $(gen_bidi_type_tab)
	(DATA_FILE_TYPE=`echo $< | sed s,.*/,,`; \
	./$(gen_bidi_type_tab) $(COMPRESSION) \
	 $$DATA_FILE_TYPE $< > $@ || ($(RM) $@ && false))

bidi-type.tab.i:
	@for x in $(BIDI_TYPE_TAB_VARIANTS); do \
	   if $(MAKE) $(AM_MAKEFLAGS) $$x && mv $$x $@; then \
	     break; \
	   fi; \
	 done; \
	 test -f $@ || (echo Could not build $@; false)

DISTCLEANFILES += $(BIDI_TYPE_TAB_VARIANTS)

# generate mirroring.tab.i

gen_mirroring_tab = gen-mirroring-tab$(EXEEXT)

MIRRORING_TAB_VARIANTS = \
		BidiMirroring_mirroring.tab.i

BidiMirroring_mirroring.tab.i: \
		BidiMirroring.txt \
		fribidi-unicode-version.h \
		$(gen_mirroring_tab_SOURCES)
	$(MAKE) $(AM_MAKEFLAGS) $(gen_mirroring_tab)
	(DATA_FILE_TYPE=`echo $< | sed s,.*/,,`; \
	./$(gen_mirroring_tab) \
	 $$DATA_FILE_TYPE $< > $@ || ($(RM) $@ && false))

mirroring.tab.i:
	@for x in $(MIRRORING_TAB_VARIANTS); do \
	   if $(MAKE) $(AM_MAKEFLAGS) $$x && mv $$x $@; then \
	     break; \
	   fi; \
	 done; \
	 test -f $@ || (echo Could not build $@; false)

DISTCLEANFILES += $(MIRRORING_TAB_VARIANTS)

# generate fribidi-unicode-version.h

gen_unicode_version = gen-unicode-version$(EXEEXT)

UNICODE_VERSION_VARIANTS = \
		ReadMe_fribidi-unicode-version.h

ReadMe_fribidi-unicode-version.h: \
		ReadMe.txt \
		$(gen_unicode_version_SOURCES)
	$(MAKE) $(AM_MAKEFLAGS) $(gen_unicode_version)
	(DATA_FILE_TYPE=`echo $< | sed s,.*/,,`; \
	./$(gen_unicode_version) \
	 $$DATA_FILE_TYPE $< > $@ || ($(RM) $@ && false))

fribidi-unicode-version.h:
	@for x in $(UNICODE_VERSION_VARIANTS); do \
	   if $(MAKE) $(AM_MAKEFLAGS) $$x && mv $$x $@; then \
	     break; \
	   fi; \
	 done; \
	 test -f $@ || (echo Could not build $@; false)

DISTCLEANFILES += $(UNICODE_VERSION_VARIANTS)

