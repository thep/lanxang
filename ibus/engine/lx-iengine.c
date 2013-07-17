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
  #include <config.h>
#endif

#include "lx-iengine.h"

G_DEFINE_INTERFACE (LxIEngine, lx_iengine, 0)

static void
lx_iengine_default_init (LxIEngineInterface *iface)
{
}

gboolean
lx_iengine_process_key_event (LxIEngine  *self,
                              IBusEngine *engine,
                              guint       keyval,
                              guint       keycode,
                              guint       modifiers)
{
  g_return_val_if_fail (LX_IS_IENGINE (self), FALSE);

  return LX_IENGINE_GET_INTERFACE(self)->process_key_event (self, engine,
                                                            keyval, keycode,
                                                            modifiers);
}

/*
vi:ts=2:nowrap:ai:expandtab
*/
