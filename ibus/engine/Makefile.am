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
	lx-tn-engine.h \
	lx-tn-engine.c \
	lx-pali-engine.h \
	lx-pali-engine.c \
	im-utils.h \
	im-utils.c \
	main.c \
	utils.h \
	utils.c \
	$(NULL)

noinst_HEADERS = \
	engine.h \
	lx-iengine.h \
	lx-tham-engine.h \
	lx-tn-engine.h \
	lx-pali-engine.h \
	im-utils.h \
	utils.h \
	$(NULL)

ibus_engine_lanxang_CFLAGS = \
	$(IBUS_CFLAGS)  \
	$(GLIB_CFLAGS) \
	-I$(top_srcdir)/src \
	$(NULL)

ibus_engine_lanxang_LDADD = \
	$(top_builddir)/src/liblanxang.a \
	libibus-lanxang-config.a \
	$(IBUS_LIBS) \
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

