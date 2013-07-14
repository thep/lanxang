AM_CPPFLAGS = \
	-DLOCALEDIR=\"$(localedir)\" \
	-DCOMPONENTDIR=\"$(componentdir)\" \
	-DPKGDATADIR=\"$(pkgdatadir)\" \
	$(NULL)

libexec_PROGRAMS = ibus-setup-lanxang-tham

ibus_setup_lanxang_tham_SOURCES = \
	tham-setup.c \
	tham-dlg-setup.c \
	$(NULL)

noinst_HEADERS = \
	tham-dlg-setup.h \
	$(NULL)

ibus_setup_lanxang_tham_CFLAGS = \
	$(IBUS_CFLAGS)  \
	$(GTK_CFLAGS) \
	-I$(top_srcdir)/ibus/engine \
	$(NULL)

ibus_setup_lanxang_tham_LDADD = \
	$(IBUS_LIBS) \
	$(GTK_LIBS) \
	$(top_builddir)/ibus/engine/libibus-lanxang-config.a \
	$(NULL)

