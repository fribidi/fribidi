
TESTS = run.tests

check_SCRIPTS = run.tests

TEST_DATAS = \
	test_CapRTL_explicit.input	\
	test_CapRTL_explicit.reference	\
	test_CapRTL_implicit.input	\
	test_CapRTL_implicit.reference	\
	test_ISO8859-8_hebrew.input	\
	test_ISO8859-8_hebrew.reference	\
	test_UTF-8_persian.input	\
	test_UTF-8_persian.reference

EXTRA_DIST =	\
	$(TESTS) \
	$(TEST_DATAS)

test_%.reference: test_%.input
	testcase="$@" && 	\
	charset="`echo "$@" | $(SED) 's/_[^_]*$$//;s/.*_//'`" &&	\
	"$(top_builddir)"/src/fribidi --test --charset "$$charset" "$<" > "$@" || \
	($(RM) "$@"; false)

test.reference:
	for testcase in "$(srcdir)"/test_*.input; do	\
		test="`echo "$$testcase"	\
		  | $(SED) 's/\.input$$/.reference/i; s|.*/||;'`" &&	\
		$(MAKE) "tests/$$test" ||	\
		exit 1;	\
	done

