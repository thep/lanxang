AM_CPPFLAGS = \
	-DLOCALEDIR=\"$(localedir)\" \
	-DCOMPONENTDIR=\"$(componentdir)\" \
	-DPKGDATADIR=\"$(pkgdatadir)\" \
	$(NULL)

libexec_PROGRAMS = ibus-setup-lanxang

ibus_setup_lanxang_SOURCES = \
	setup.c \
	dlg-setup.c \
	$(NULL)

noinst_HEADERS = \
	dlg-setup.h \
	$(NULL)

ibus_setup_lanxang_CFLAGS = \
	$(IBUS_CFLAGS)  \
	$(GTK_CFLAGS) \
	-I$(top_srcdir)/ibus/engine \
	$(NULL)

ibus_setup_lanxang_LDADD = \
	$(IBUS_LIBS) \
	$(GTK_LIBS) \
	$(top_builddir)/ibus/engine/libibus-lanxang-config.a \
	$(NULL)

