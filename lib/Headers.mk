libfribidi_la_headers = \
		fribidi-begindecls.h \
		fribidi-bidi.h \
		fribidi-bidi-type.h \
		fribidi-bidi-types.h \
		fribidi-bidi-types-list.h \
		fribidi-common.h \
		fribidi-enddecls.h \
		fribidi-env.h \
		fribidi-joining.h \
		fribidi-joining-type.h \
		fribidi-joining-types.h \
		fribidi-joining-types-list.h \
		fribidi-mirroring.h \
		fribidi-types.h \
		fribidi-unicode.h \
		fribidi-unicode-version.h \
		fribidi.h

libfribidi_la_symbols = $(shell cat $(top_srcdir)/lib/fribidi.def)
