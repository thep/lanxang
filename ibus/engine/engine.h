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

#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <ibus.h>

typedef enum _IscMode IscMode;

enum _IscMode
{
  ISC_PASSTHROUGH,
  ISC_BASIC,
  ISC_STRICT,
};


#define IBUS_TYPE_LANXANG_ENGINE  (ibus_lanxang_engine_get_type())
#define IBUS_LANXANG_ENGINE(obj) \
          (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                       IBUS_TYPE_LANXANG_ENGINE, \
                                       IBusLanXangEngine))
#define IBUS_LANXANG_ENGINE_CLASS(klass) \
          (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                    IBUS_TYPE_LANXANG_ENGINE, \
                                    IBusLanXangEngineClass))
#define IBUS_IS_LANXANG_ENGINE(obj) \
          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                       IBUS_TYPE_LANXANG_ENGINE))
#define IBUS_IS_LANXANG_ENGINE_CLASS(klass) \
          (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                    IBUS_TYPE_LANXANG_ENGINE))
#define IBUS_LANXANG_ENGINE_GET_CLASS(obj) \
          (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                      IBUS_TYPE_LANXANG_ENGINE, \
                                      IBusLanXangEngineClass))

GType   ibus_lanxang_engine_get_type ();

void ibus_lanxang_init (void);

#endif /* __ENGINE_H__ */

/*
vi:ts=2:nowrap:ai:expandtab
*/
