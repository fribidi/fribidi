##                                               -*- Automake -*-
## Process this file with automake to produce Makefile.in
##############################################################################
## $Id: Makefile.am,v 1.6 2004-05-31 18:43:26 behdad Exp $
## $Author: behdad $
## $Date: 2004-05-31 18:43:26 $
## $Revision: 1.6 $
## $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/Makefile.am,v $
##############################################################################

## The order of subdirs is important, don't change without a reason.
SUBDIRS = gen.tab charset lib bin doc test

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = @PACKAGE@.pc

MAINTAINERCLEANFILES = \
		$(srcdir)/INSTALL \
		$(srcdir)/aclocal.m4 \
		$(srcdir)/autoscan.log \
		$(srcdir)/compile \
		$(srcdir)/config.guess \
		$(srcdir)/config.h.in \
		$(srcdir)/config.sub \
		$(srcdir)/configure \
		$(srcdir)/configure.scan \
		$(srcdir)/depcomp \
		$(srcdir)/install-sh \
		$(srcdir)/ltmain.sh \
		$(srcdir)/missing \
		`find "$(srcdir)" -type f -name Makefile.in -print` \
		`find "$(srcdir)" -type f -name "*~" -print`

BUILT_SOURCES = lib/fribidi-unicode-version.h
lib/fribidi-unicode-version.h:
	(cd lib && $(MAKE) $(AM_MAKEFLAGS) fribidi-unicode-version.h)

# Indent all C source and header files

.PHONY: indent

indent: $(srcdir)/.indent.pro
	cd "$(srcdir)" && \
	find . -type f -mindepth 2 -name "*.[ch]" -print | \
		xargs ./missing --run indent


# Creating ChangeLog from CVS log, with cvs2cl:

MAINTAINERCLEANFILES += ChangeLog ChangeLog.bak

EXTRA_DIST = ChangeLog

ChangeLog:
	if test -d "$(srcdir)/CVS"; then \
		cd "$(srcdir)" && \
		./missing --run cvs2cl --stdout --utc --FSF -U AUTHORS; \
	fi > $@
