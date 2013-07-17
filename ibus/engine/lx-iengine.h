/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 *  Copyright (C) 2013 Theppitak Karoonboonyanan <thep@linux.thai.net>
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

#ifndef __LX_IENGINE_H__
#define __LX_IENGINE_H__

#include <glib-object.h>
#include <ibus.h>

#define LX_TYPE_IENGINE  (lx_iengine_get_type())
#define LX_IENGINE(obj) \
          (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                       LX_TYPE_IENGINE, \
                                       LxIEngine))
#define LX_IS_IENGINE(obj) \
          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                       LX_TYPE_IENGINE))
#define LX_IENGINE_GET_INTERFACE(inst) \
          (G_TYPE_INSTANCE_GET_INTERFACE ((inst), \
                                          LX_TYPE_IENGINE, \
                                          LxIEngineInterface))

typedef struct _LxIEngine LxIEngine;
typedef struct _LxIEngineInterface LxIEngineInterface;

struct _LxIEngineInterface
{
  GTypeInterface parent_iface;

  /* Virtual functions */
  gboolean (*process_key_event) (LxIEngine  *self,
                                 IBusEngine *engine,
                                 guint       keyval,
                                 guint       keycode,
                                 guint       modifiers);
};

GType   lx_iengine_get_type ();

gboolean lx_iengine_process_key_event (LxIEngine  *self,
                                       IBusEngine *engine,
                                       guint       keyval,
                                       guint       keycode,
                                       guint       modifiers);

#endif /* __LX_IENGINE_H__ */

/*
vi:ts=2:nowrap:ai:expandtab
*/
