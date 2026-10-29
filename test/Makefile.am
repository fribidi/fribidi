
SUBDIRS = unicode-conformance

TESTS_ENVIRONMENT = \
	srcdir=${srcdir}; export srcdir; \
	top_builddir=${top_builddir}; export top_builddir; \
	EXEEXT=${EXEEXT}; export EXEEXT;

AM_TESTS_ENVIRONMENT = $(TESTS_ENVIRONMENT)

TESTS = run.tests test-reorder-nsm test-par-direction test-isolate-chain

check_SCRIPTS = run.tests

check_PROGRAMS = test-reorder-nsm test-par-direction test-isolate-chain

test_reorder_nsm_SOURCES = test-reorder-nsm.c
test_par_direction_SOURCES = test-par-direction.c
test_isolate_chain_SOURCES = test-isolate-chain.c

AM_CPPFLAGS = \
		@FRIBIDI_CPPFLAGS@ \
		-I$(top_builddir)/lib \
		-I$(top_srcdir)/lib

LDADD = $(top_builddir)/lib/libfribidi.la

TEST_DATAS = \
	test_CapRTL_explicit.input \
	test_CapRTL_explicit.reference \
	test_CapRTL_implicit.input \
	test_CapRTL_implicit.reference \
	test_CapRTL_isolate.input \
	test_CapRTL_isolate.reference \
	test_ISO8859-8_hebrew.input \
	test_ISO8859-8_hebrew.reference \
	test_UTF-8_persian.input \
	test_UTF-8_persian.reference \
	test_UTF-8_reordernsm.input \
	test_UTF-8_reordernsm.reference


EXTRA_DIST = \
	run.tests \
	$(TEST_DATAS)

# Regenerate the reference files from the inputs.  This is deliberately a
# plain phony target and not a suffix rule: a rule that rebuilds the
# distributed .input files makes "make dist" overwrite them whenever a
# .reference happens to have a newer mtime.
references:
	for input in "$(srcdir)"/test_*.input; do \
		base="`echo "$$input" | sed 's|.*/||; s/\.input$$//'`" && \
		charset="`echo "$$base" | sed 's/^test_//; s/_.*//'`" && \
		$(top_builddir)/bin/fribidi --test --charset "$$charset" "$$input" \
		  > "$(srcdir)/$$base.reference" || exit 1; \
	done

.PHONY: references

-include $(top_srcdir)/git.mk
