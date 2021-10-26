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

#include "engine.h"
#include "lx-iengine.h"
#include "lx-tham-engine.h"
#include "lx-tn-engine.h"
#include "lx-pali-engine.h"
#include "ibus-config.h"
#include <glib.h>
#include <string.h>

#define N_ELM(ar)  (sizeof(ar)/sizeof((ar)[0]))

typedef struct _IBusLanXangEngine IBusLanXangEngine;
typedef struct _IBusLanXangEngineClass IBusLanXangEngineClass;

struct _IBusLanXangEngine
{
  IBusEngine parent;

  /* members */
  LxIEngine *lx_iengine;
};

struct _IBusLanXangEngineClass
{
  IBusEngineClass parent;
};

static GObject*
ibus_lanxang_engine_constructor (GType                  type,
                                 guint                  n_construct_params,
                                 GObjectConstructParam *construct_params);
static void ibus_lanxang_engine_destroy (IBusLanXangEngine *lanxang_engine);

static void ibus_lanxang_engine_enable (IBusEngine *engine);
static gboolean ibus_lanxang_engine_process_key_event (IBusEngine *engine,
                                                       guint       keyval,
                                                       guint       keycode,
                                                       guint       modifiers);

G_DEFINE_TYPE (IBusLanXangEngine, ibus_lanxang_engine, IBUS_TYPE_ENGINE)

void
ibus_lanxang_init (void)
{
  ibus_lanxang_init_config ();
}


static void
ibus_lanxang_engine_class_init (IBusLanXangEngineClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  IBusObjectClass *ibus_object_class = IBUS_OBJECT_CLASS (klass);
  IBusEngineClass *engine_class = IBUS_ENGINE_CLASS (klass);

  object_class->constructor = ibus_lanxang_engine_constructor;
  ibus_object_class->destroy
    = (IBusObjectDestroyFunc) ibus_lanxang_engine_destroy;
  engine_class->enable = ibus_lanxang_engine_enable;
  engine_class->process_key_event = ibus_lanxang_engine_process_key_event;
}

static GObject*
ibus_lanxang_engine_constructor (GType                  type,
                                 guint                  n_construct_params,
                                 GObjectConstructParam *construct_params)
{
  IBusLanXangEngine *engine;
  const gchar *name;

  engine = (IBusLanXangEngine *)
             G_OBJECT_CLASS (ibus_lanxang_engine_parent_class)->constructor (
               type, n_construct_params, construct_params);
  name = ibus_engine_get_name (IBUS_ENGINE (engine));

  if (name && strcmp (name, "lanxang-tham") == 0)
    {
      engine->lx_iengine = g_object_new (LX_TYPE_THAM_ENGINE, NULL);
    }
  else if (name && strcmp (name, "lanxang-tn") == 0)
    {
      engine->lx_iengine = g_object_new (LX_TYPE_TN_ENGINE, NULL);
    }
  else if (name && strcmp (name, "lanxang-pali") == 0)
    {
      engine->lx_iengine = g_object_new (LX_TYPE_PALI_ENGINE, NULL);
    }

  return G_OBJECT (engine);
}

static void
ibus_lanxang_engine_destroy (IBusLanXangEngine *lanxang_engine)
{
  g_object_unref (lanxang_engine->lx_iengine);
  IBUS_OBJECT_CLASS (ibus_lanxang_engine_parent_class)->destroy (
                      IBUS_OBJECT (lanxang_engine));
}

static void
ibus_lanxang_engine_init (IBusLanXangEngine *lanxang_engine)
{
  if (g_object_is_floating (lanxang_engine))
    {
      g_object_ref_sink (lanxang_engine);
    }
}

static void
ibus_lanxang_engine_enable (IBusEngine *engine)
{
  /* dummy call to tell the input context that the engine will utilize
     surrounding-text */
  ibus_engine_get_surrounding_text (engine, NULL, NULL, NULL);
}

static gboolean
ibus_lanxang_engine_process_key_event (IBusEngine *engine,
                                       guint       keyval,
                                       guint       keycode,
                                       guint       modifiers)
{
  IBusLanXangEngine *lanxang_engine = IBUS_LANXANG_ENGINE (engine);

  return lx_iengine_process_key_event (lanxang_engine->lx_iengine, engine,
                                       keyval, keycode, modifiers);
}

/*
vi:ts=2:nowrap:ai:expandtab
*/
