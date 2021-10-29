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

#include "ibus-config.h"
#include <gio/gio.h>

#include <stdlib.h>

/* Global variables */
static GSettings *ibus_lanxang_settings = NULL;

void
ibus_lanxang_init_config (void)
{
  if (!ibus_lanxang_settings)
    {
      ibus_lanxang_settings = g_settings_new (CONFIG_SCHEMA);
    }
}

void
ibus_lanxang_read_config (IBusLanXangSetupOptions *opt)
{
  ibus_lanxang_read_tham_config (&opt->tham_options);
  ibus_lanxang_read_tn_config (&opt->tn_options);
  ibus_lanxang_read_pali_config (&opt->pali_options);
}

void
ibus_lanxang_write_config (const IBusLanXangSetupOptions *opt)
{
  ibus_lanxang_write_tham_config (&opt->tham_options);
  ibus_lanxang_write_tn_config (&opt->tn_options);
  ibus_lanxang_write_pali_config (&opt->pali_options);
}


void
ibus_lanxang_read_tham_config (IBusLanXangThamOptions *opt)
{
  opt->isc_mode = g_settings_get_enum (ibus_lanxang_settings,
                                       CONFIG_THAM_ISC_MODE);
}

void
ibus_lanxang_write_tham_config (const IBusLanXangThamOptions *opt)
{
  g_settings_set_enum (ibus_lanxang_settings,
                       CONFIG_THAM_ISC_MODE, opt->isc_mode);
}


void
ibus_lanxang_read_tn_config (IBusLanXangTNOptions *opt)
{
  opt->isc_mode = g_settings_get_enum (ibus_lanxang_settings,
                                       CONFIG_TN_ISC_MODE);
}

void
ibus_lanxang_write_tn_config (const IBusLanXangTNOptions *opt)
{
  g_settings_set_enum (ibus_lanxang_settings,
                       CONFIG_TN_ISC_MODE, opt->isc_mode);
}


void
ibus_lanxang_read_pali_config (IBusLanXangPaliOptions *opt)
{
  opt->isc_mode = g_settings_get_enum (ibus_lanxang_settings,
                                       CONFIG_PALI_ISC_MODE);
}

void
ibus_lanxang_write_pali_config (const IBusLanXangPaliOptions *opt)
{
  g_settings_set_enum (ibus_lanxang_settings,
                       CONFIG_PALI_ISC_MODE, opt->isc_mode);
}


/*
vi:ts=2:nowrap:ai:expandtab
*/
