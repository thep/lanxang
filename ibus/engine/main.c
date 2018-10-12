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

#include <stdlib.h>
#include <ibus.h>
#include "engine.h"
#include "utils.h"

static IBusBus *bus = NULL;
static IBusFactory *factory = NULL;

/* command line options */
static gboolean opt_ibus = FALSE;
static gboolean opt_verbose = FALSE;

static const GOptionEntry entries[] =
{
  { "ibus", 'i', 0, G_OPTION_ARG_NONE, &opt_ibus,
    "component is executed by ibus", NULL },
  { "verbose", 'v', 0, G_OPTION_ARG_NONE, &opt_verbose,
    "verbose", NULL },
  { NULL },
};

static void
ibus_disconnected_cb (IBusBus  *bus,
                      gpointer  user_data)
{
  ibus_quit ();
}


static gboolean
init (void)
{
  ibus_init ();

  bus = ibus_bus_new ();
  g_object_ref_sink (bus);
  g_signal_connect (bus, "disconnected", G_CALLBACK (ibus_disconnected_cb),
                    NULL);
	
  ibus_lanxang_init (bus);

  factory = ibus_factory_new (ibus_bus_get_connection (bus));
  g_object_ref_sink (factory);
  ibus_factory_add_engine (factory, "lanxang-tham", IBUS_TYPE_LANXANG_ENGINE);
  ibus_factory_add_engine (factory, "lanxang-tn", IBUS_TYPE_LANXANG_ENGINE);
  ibus_factory_add_engine (factory, "lanxang-pali", IBUS_TYPE_LANXANG_ENGINE);

  if (opt_ibus)
    {
      ibus_bus_request_name (bus, "org.freedesktop.IBus.LanXang", 0);
    }
  else
    {
      IBusComponent *component;

      component = ibus_lanxang_get_component ();
      if (!component)
        {
          g_print ("Cannot create LanXang component.\n");
          return FALSE;
        }
      ibus_bus_register_component (bus, component);
    }

  return TRUE;
}

int
main (int argc, char **argv)
{
  GError *error = NULL;
  GOptionContext *context;

  /* Parse the command line */
  context = g_option_context_new ("- ibus LanXang engine");
  g_option_context_add_main_entries (context, entries, "ibus-lanxang");

  if (!g_option_context_parse (context, &argc, &argv, &error))
    {
      g_print ("Option parsing failed: %s\n", error->message);
      g_error_free (error);
      return (-1);
    }

  /* Go */
  if (!init ())
    exit (1);

  ibus_main ();
}

/*
vi:ts=2:nowrap:ai:expandtab
*/
