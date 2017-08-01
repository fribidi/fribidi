
TESTS_ENVIRONMENT = \
	srcdir=${srcdir}; export srcdir; \
	top_builddir=${top_builddir}; export top_builddir; \
	EXEEXT=${EXEEXT}; export EXEEXT;

AM_TESTS_ENVIRONMENT = $(TESTS_ENVIRONMENT)

AM_CPPFLAGS = \
		-I$(top_builddir)/lib \
		-I$(top_srcdir)/lib \
		-I$(top_srcdir)/charset \
		$(MISC_CFLAGS)
LDADD = $(top_builddir)/lib/libfribidi.la $(MISC_LIBS)

if FRIBIDI_USE_GLIB
check_PROGRAMS = test test-character
bin_PROGRAMS = $(check_PROGRAMS)
#TESTS = \
#	test \
#	test-character
endif

test_SOURCES = test.c
test_character_SOURCES = test-character.c

-include $(top_srcdir)/git.mk
