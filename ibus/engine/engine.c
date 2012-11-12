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
#include "lx-kbd.h"
#include <glib.h>
#include <string.h>

typedef struct _IBusLanXangEngine IBusLanXangEngine;
typedef struct _IBusLanXangEngineClass IBusLanXangEngineClass;

struct _IBusLanXangEngine
{
  IBusEngine parent;

  /* members here */
};

struct _IBusLanXangEngineClass
{
  IBusEngineClass parent;
};

static void ibus_lanxang_engine_class_init (IBusLanXangEngineClass *klass);
static void ibus_lanxang_engine_init (IBusLanXangEngine *lanxang_engine);
static gboolean ibus_lanxang_engine_process_key_event (IBusEngine *engine,
                                                       guint       keyval,
                                                       guint       keycode,
                                                       guint       modifiers);

/* Utility functions */
static gboolean
is_client_support_surrounding (IBusEngine *engine);

/* Global variables */
static IBusConfig *ibus_config = NULL;

void
ibus_lanxang_init (IBusBus *bus)
{
  ibus_config = ibus_bus_get_config (bus);
}


GType
ibus_lanxang_engine_get_type ()
{
  static GType type = 0;

  static const GTypeInfo type_info =
    {
      sizeof (IBusLanXangEngineClass),
      (GBaseInitFunc)         NULL,
      (GBaseFinalizeFunc)     NULL,
      (GClassInitFunc)        ibus_lanxang_engine_class_init,
      (GClassFinalizeFunc)    NULL,
      NULL,
      sizeof (IBusLanXangEngine),
      0,
      (GInstanceInitFunc)     ibus_lanxang_engine_init,
    };

  if (!type)
    {
      type = g_type_register_static (IBUS_TYPE_ENGINE,
                                     "IBusLanXangEngine",
                                     &type_info,
                                     (GTypeFlags)0);
    }

  return type;
}

static void
ibus_lanxang_engine_class_init (IBusLanXangEngineClass *klass)
{
  IBusEngineClass *engine_class = IBUS_ENGINE_CLASS (klass);

  engine_class->process_key_event = ibus_lanxang_engine_process_key_event;
}

static void
ibus_lanxang_engine_init (IBusLanXangEngine *lanxang_engine)
{
}

static gboolean
is_context_lost_key (guint keyval)
{
  return ((keyval & 0xFF00) == 0xFF00) &&
         (keyval == IBUS_BackSpace ||
          keyval == IBUS_Tab ||
          keyval == IBUS_Linefeed ||
          keyval == IBUS_Clear ||
          keyval == IBUS_Return ||
          keyval == IBUS_Pause ||
          keyval == IBUS_Scroll_Lock ||
          keyval == IBUS_Sys_Req ||
          keyval == IBUS_Escape ||
          keyval == IBUS_Delete ||
          /* IsCursorkey */
          (IBUS_Home <= keyval && keyval <= IBUS_Begin) ||
          /* IsKeypadKey, non-chars only */
          (IBUS_KP_Space <= keyval && keyval <= IBUS_KP_Delete) ||
          /* IsMiscFunctionKey */
          (IBUS_Select <= keyval && keyval <= IBUS_Break) ||
          /* IsFunctionKey */
          (IBUS_F1 <= keyval && keyval <= IBUS_F35));
}

static gboolean
is_context_intact_key (guint keyval)
{
  return (((keyval & 0xFF00) == 0xFF00) &&
          ( /* IsModifierKey */
           (IBUS_Shift_L <= keyval && keyval <= IBUS_Hyper_R) ||
           (keyval == IBUS_Mode_switch) ||
           (keyval == IBUS_Num_Lock))) ||
         (((keyval & 0xFE00) == 0xFE00) &&
          (IBUS_ISO_Lock <= keyval && keyval <= IBUS_ISO_Last_Group_Lock));
}

static gboolean
ibus_lanxang_engine_process_key_event (IBusEngine *engine,
                                       guint       keyval,
                                       guint       keycode,
                                       guint       modifiers)
{
  IBusLanXangEngine *lanxang_engine = IBUS_LANXANG_ENGINE (engine);
  gunichar new_char;
  IBusText *text;

  if (modifiers & IBUS_RELEASE_MASK)
    return FALSE;

  if (modifiers & (IBUS_CONTROL_MASK | IBUS_MOD1_MASK)
      || is_context_lost_key (keyval))
    {
      return FALSE;
    }
  if (0 == keyval || is_context_intact_key (keyval))
    {
      return FALSE;
    }

  new_char = lx_map_key (keyval, modifiers & IBUS_MOD5_MASK);
  text = ibus_text_new_from_unichar (new_char);

  ibus_engine_commit_text (engine, text);
  return TRUE;
}

/*
vi:ts=2:nowrap:ai:expandtab
*/
