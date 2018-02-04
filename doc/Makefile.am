dist_man_MANS =
dist_noinst_MANS =
EXTRA_DIST =
MOSTLYCLEANFILES =
MAINTAINERCLEANFILES =

if FRIBIDI_ENABLE_DOC

doxygen.stamp:
	@$(RM) $@.tmp
	@touch $@.tmp
	@echo Running Doxygen
	@$(DOXYGEN) Doxyfile
	@mv $@.tmp $@

directory = $(builddir)/man3

dist_man_MANS += $(directory)/fribidi-bidi.h.3

$(dist_man_MANS): doxygen.stamp
	@if ! test -f $@; then \
		$(RM) $<; \
		$(MAKE) $(AM_MAKEFLAGS) $<;\
	fi
	@touch $@

EXTRA_DIST += doxygen.stamp
MOSTLYCLEANFILES += doxygen.stamp.tmp
MAINTAINERCLEANFILES += $(dist_man_MANS) doxygen.stamp

endif # FRIBIDI_ENABLE_DOC

-include $(top_srcdir)/git.mk
