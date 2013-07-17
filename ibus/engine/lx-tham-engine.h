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

#ifndef __LX_THAM_ENGINE_H__
#define __LX_THAM_ENGINE_H__

#include <glib-object.h>

#define LX_TYPE_THAM_ENGINE  (lx_tham_engine_get_type())
#define LX_THAM_ENGINE(obj) \
          (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                       LX_TYPE_THAM_ENGINE, \
                                       LxThamEngine))
#define LX_THAM_ENGINE_CLASS(klass) \
          (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                    LX_TYPE_THAM_ENGINE, \
                                    LxThamEngineClass))
#define LX_IS_THAM_ENGINE(obj) \
          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                       LX_TYPE_THAM_ENGINE))
#define LX_IS_THAM_ENGINE_CLASS(klass) \
          (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                    LX_TYPE_THAM_ENGINE))
#define LX_THAM_ENGINE_GET_CLASS(obj) \
          (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                      LX_TYPE_THAM_ENGINE, \
                                      LxThamEngineClass))

GType   lx_tham_engine_get_type ();

#endif /* __LX_THAM_ENGINE_H__ */

/*
vi:ts=2:nowrap:ai:expandtab
*/
