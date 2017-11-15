
TESTS_ENVIRONMENT = \
	srcdir=${srcdir}; export srcdir; \
	top_builddir=${top_builddir}; export top_builddir; \
	builddir=${builddir}; export builddir; \
	EXEEXT=${EXEEXT}; export EXEEXT;

AM_TESTS_ENVIRONMENT = $(TESTS_ENVIRONMENT)

TEST_EXTENSIONS = .txt

TXT_LOG_COMPILER = sh $(srcdir)/run.tests

AM_CPPFLAGS = \
		-I$(top_builddir)/lib \
		-I$(top_srcdir)/lib \
		-I$(top_srcdir)/charset \
		$(GLIB_CFLAGS)
LDADD = $(top_builddir)/lib/libfribidi.la $(GLIB_LIBS)

if FRIBIDI_USE_GLIB
check_PROGRAMS = test test-character
bin_PROGRAMS = $(check_PROGRAMS)
TESTS = \
	BidiTest.txt \
	BidiCharacterTest.txt
endif

test_SOURCES = test.c
test_character_SOURCES = test-character.c

-include $(top_srcdir)/git.mk
