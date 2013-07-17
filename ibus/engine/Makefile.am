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
	lx-iengine.h \
	lx-iengine.c \
	lx-tham-engine.h \
	lx-tham-engine.c \
	main.c \
	utils.h \
	utils.c \
	$(NULL)

noinst_HEADERS = \
	engine.h \
	lx-iengine.h \
	lx-tham-engine.h \
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
	libibus-lanxang-config.a \
	$(NULL)

noinst_LIBRARIES = libibus-lanxang-config.a

libibus_lanxang_config_a_SOURCES = \
	ibus-config.c \
	$(NULL)

libibus_lanxang_config_a_CFLAGS = \
	$(IBUS_CFLAGS) \
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

