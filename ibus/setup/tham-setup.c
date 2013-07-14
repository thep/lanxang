/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 *  Copyright (C) 2012 Theppitak Karoonboonyanan <thep@linux.thai.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <libintl.h>
#include <locale.h>
#include <gtk/gtk.h>
#include <ibus.h>
#include <stdlib.h>
#include "ibus-config.h"
#include "tham-dlg-setup.h"

#define _(string) gettext(string)

static gboolean opt_engine = FALSE;
static gboolean opt_version = FALSE;

static const GOptionEntry entries[] =
{
  { "engine", 'x', 0, G_OPTION_ARG_NONE, &opt_engine,
    "indicate setup run from ibus-engine-lanxang", NULL },
  { "version", 'V', 0, G_OPTION_ARG_NONE, &opt_version,
    "print ibus-lanxang version", NULL },
  { NULL },
};

static void
ibus_lanxang_tham_disconnected_cb (IBusBus* bus, gpointer user_data)
{
  exit(2);
}

int
main (int argc, char **argv)
{
  GError     *error = NULL;
  GOptionContext *context;

  GtkWidget  *main_dlg;
  IBusConfig *config;
  IBusBus    *bus;
  IBusLanXangSetupOptions opt;
  int         ret;

  setlocale (LC_ALL, "");
  bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
  textdomain (GETTEXT_PACKAGE);

  context = g_option_context_new ("- ibus lanxang Tham setup component");
  g_option_context_add_main_entries (context, entries, "ibus-lanxang");
  g_option_context_parse (context, &argc, &argv, &error);

  if (opt_version)
    {
      g_print (
        PACKAGE " (setup component) " VERSION "\n"
        "Copyright (C) 2012 Theppitak Karoonboonyanan <thep@linux.thai.net>\n"
        "License: GNU GPL2 or later <http://www.gnu.org/licenses/gpl-2.0.html>\n"
        "\n"
        "This is free software; see the source for copying conditions.\n"
        "There is NO WARRANTY; not even for MERCHANTABILITY or FITNESS FOR\n"
        "A PARTICULAR PURPOSE.\n"
      );
      return 0;
    }

  gtk_init (&argc, &argv);

  gtk_window_set_default_icon_from_file (PKGDATADIR"/icons/ibus-lanxang-tham.svg",
                                         NULL);
  main_dlg = ibus_lanxang_tham_setup_dialog_new ();
  g_signal_connect (main_dlg, "destroy", gtk_main_quit, NULL);

  ibus_init ();
  bus = ibus_bus_new ();
  g_signal_connect (bus, "disconnected",
                    G_CALLBACK (ibus_lanxang_tham_disconnected_cb), NULL);
  config = ibus_bus_get_config (bus);

  ibus_lanxang_read_config (config, &opt);
  ibus_lanxang_tham_setup_set_values (GTK_DIALOG (main_dlg), &opt);

  ret = gtk_dialog_run (GTK_DIALOG (main_dlg));
  if (GTK_RESPONSE_OK == ret)
    {
      ibus_lanxang_tham_setup_get_values (GTK_DIALOG (main_dlg), &opt);
      ibus_lanxang_write_config (config, &opt);
      if (!opt_engine)
        {
          force_engine_to_reload_config ();
        }
    }

  return 0;
}

/*
vi:ts=2:nowrap:ai:expandtab
*/
