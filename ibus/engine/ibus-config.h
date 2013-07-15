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

#ifndef __IBUS_CONFIG_H__
#define __IBUS_CONFIG_H__

#include "engine.h"
#include <ibus.h>

/* Configuration names */
#define CONFIG_SECTION  "engine/LanXang"
#define CONFIG_THAM_ISC_MODE "tham_isc_mode"

typedef struct _IBusLanXangSetupOptions IBusLanXangSetupOptions;

struct _IBusLanXangSetupOptions
{
  IscMode tham_isc_mode;
};

void ibus_lanxang_init_config (IBusBus *bus);

void ibus_lanxang_read_config (IBusLanXangSetupOptions *opt);
void ibus_lanxang_write_config (const IBusLanXangSetupOptions *opt);
gboolean force_engine_to_reload_config ();

#endif /* __IBUS_CONFIG_H__ */

/*
vi:ts=2:nowrap:ai:expandtab
*/
