AM_CPPFLAGS = \
	-DLOCALEDIR=\"$(localedir)\" \
	-DCOMPONENTDIR=\"$(componentdir)\" \
	-DPKGDATADIR=\"$(pkgdatadir)\" \
	$(NULL)

libexec_PROGRAMS = \
	ibus-setup-lanxang-tham \
	ibus-setup-lanxang-tn \
	ibus-setup-lanxang-pali \
	$(NULL)

noinst_HEADERS = \
	tham-dlg-setup.h \
	tn-dlg-setup.h \
	pali-dlg-setup.h \
	$(NULL)

ibus_setup_lanxang_tham_SOURCES = \
	tham-setup.c \
	tham-dlg-setup.c \
	$(NULL)

ibus_setup_lanxang_tham_CFLAGS = \
	$(IBUS_CFLAGS)  \
	$(GTK_CFLAGS) \
	-I$(top_srcdir)/ibus/engine \
	$(NULL)

ibus_setup_lanxang_tham_LDADD = \
	$(top_builddir)/ibus/engine/libibus-lanxang-config.a \
	$(IBUS_LIBS) \
	$(GTK_LIBS) \
	$(NULL)

ibus_setup_lanxang_tn_SOURCES = \
	tn-setup.c \
	tn-dlg-setup.c \
	$(NULL)

ibus_setup_lanxang_tn_CFLAGS = \
	$(IBUS_CFLAGS)  \
	$(GTK_CFLAGS) \
	-I$(top_srcdir)/ibus/engine \
	$(NULL)

ibus_setup_lanxang_tn_LDADD = \
	$(top_builddir)/ibus/engine/libibus-lanxang-config.a \
	$(IBUS_LIBS) \
	$(GTK_LIBS) \
	$(NULL)

ibus_setup_lanxang_pali_SOURCES = \
	pali-setup.c \
	pali-dlg-setup.c \
	$(NULL)

ibus_setup_lanxang_pali_CFLAGS = \
	$(IBUS_CFLAGS)  \
	$(GTK_CFLAGS) \
	-I$(top_srcdir)/ibus/engine \
	$(NULL)

ibus_setup_lanxang_pali_LDADD = \
	$(top_builddir)/ibus/engine/libibus-lanxang-config.a \
	$(IBUS_LIBS) \
	$(GTK_LIBS) \
	$(NULL)

