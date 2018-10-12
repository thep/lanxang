/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 *  Copyright (C) 2018 Theppitak Karoonboonyanan <theppitak@gmail.com>
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

#ifndef __LX_PALI_ENGINE_H__
#define __LX_PALI_ENGINE_H__

#include <glib-object.h>

#define LX_TYPE_PALI_ENGINE  (lx_pali_engine_get_type())
#define LX_PALI_ENGINE(obj) \
          (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                       LX_TYPE_PALI_ENGINE, \
                                       LxPaliEngine))
#define LX_PALI_ENGINE_CLASS(klass) \
          (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                    LX_TYPE_PALI_ENGINE, \
                                    LxPaliEngineClass))
#define LX_IS_PALI_ENGINE(obj) \
          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                       LX_TYPE_PALI_ENGINE))
#define LX_IS_PALI_ENGINE_CLASS(klass) \
          (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                    LX_TYPE_PALI_ENGINE))
#define LX_PALI_ENGINE_GET_CLASS(obj) \
          (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                      LX_TYPE_PALI_ENGINE, \
                                      LxPaliEngineClass))

GType   lx_pali_engine_get_type ();

#endif /* __LX_PALI_ENGINE_H__ */

/*
vi:ts=2:nowrap:ai:expandtab
*/
