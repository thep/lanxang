componentdir = $(datadir)/ibus/component

AM_CPPFLAGS = \
	-DLOCALEDIR=\"$(localedir)\" \
	-DCOMPONENTDIR=\"$(componentdir)\" \
	-DPKGDATADIR=\"$(pkgdatadir)\" \
	$(NULL)

libexec_PROGRAMS = ibus-engine-lanxang

ibus_engine_lanxang_SOURCES = \
	engine.h \
	engine.c \
	main.c \
	utils.h \
	utils.c \
	$(NULL)

noinst_HEADERS = \
	engine.h \
	utils.h \
	$(NULL)

ibus_engine_lanxang_CFLAGS = \
	$(IBUS_CFLAGS)  \
	$(GLIB_CFLAGS) \
	-I$(top_srcdir)/src \
	$(NULL)

ibus_engine_lanxang_LDADD = \
	$(IBUS_LIBS) \
	$(GLIB_LIBS) \
	$(top_builddir)/src/liblanxang.a \
	$(NULL)

component_DATA = lanxang.xml

EXTRA_DIST = lanxang.xml.in

CLEANFILES = lanxang.xml

lanxang.xml: lanxang.xml.in

SUFFIXES = .xml.in .xml
.xml.in.xml:
	( \
		libexecdir=${libexecdir}; \
		datadir=${datadir}; \
		pkgdatadir=${pkgdatadir}; \
		s=`cat $<`; \
		eval "echo \"$${s}\""; \
	) > $@

