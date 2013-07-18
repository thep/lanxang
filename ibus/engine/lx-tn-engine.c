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
#include "lx-tn-engine.h"
#include "lx-tn-kbd.h"
#include <glib.h>
#include <string.h>

#define N_ELM(ar)  (sizeof(ar)/sizeof((ar)[0]))

typedef struct _LxTNEngine LxTNEngine;
typedef struct _LxTNEngineClass LxTNEngineClass;

#define PREEDIT_BUFF_LEN 4
struct _LxTNEngine
{
  GObject  parent;
};

struct _LxTNEngineClass
{
  GObjectClass parent;
};

static void lx_iengine_interface_init (LxIEngineInterface *iface);
static gboolean lx_tn_engine_process_key_event (LxIEngine  *lx_iengine,
                                                IBusEngine *engine,
                                                guint       keyval,
                                                guint       keycode,
                                                guint       modifiers);


G_DEFINE_TYPE_WITH_CODE (LxTNEngine, lx_tn_engine, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (LX_TYPE_IENGINE,
                                                lx_iengine_interface_init));

static void
lx_iengine_interface_init (LxIEngineInterface *iface)
{
  iface->process_key_event = lx_tn_engine_process_key_event;
}

static void
lx_tn_engine_class_init (LxTNEngineClass *klass)
{
}

static void
lx_tn_engine_init (LxTNEngine *lx_tn_engine)
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
lx_tn_engine_commit_char (LxTNEngine *lx_tn_engine,
                          IBusEngine *ibus_engine,
                          gunichar    new_char)
{
  IBusText *text;

  text = ibus_text_new_from_unichar (new_char);
  ibus_engine_commit_text (ibus_engine, text);

  return TRUE;
}

static gboolean
lx_tn_engine_process_key_event (LxIEngine  *lx_iengine,
                                IBusEngine *ibus_engine,
                                guint       keyval,
                                guint       keycode,
                                guint       modifiers)
{
  LxTNEngine *lx_tn_engine = LX_TN_ENGINE (lx_iengine);
  gint shift_lv;
  gunichar new_char;

  if (modifiers & IBUS_RELEASE_MASK)
    return FALSE;

  if (modifiers & (IBUS_CONTROL_MASK | IBUS_MOD1_MASK))
    return FALSE;

  if (is_context_lost_key (keyval))
    return FALSE;

  if (0 == keyval || is_context_intact_key (keyval))
    return FALSE;

  shift_lv = !(modifiers & (IBUS_SHIFT_MASK | IBUS_MOD5_MASK)) ? 0
               : ((modifiers & IBUS_MOD5_MASK) ? 2 : 1);
  new_char = lx_tn_map_keycode (keycode, shift_lv);
  if (0 == new_char)
    return FALSE;

  lx_tn_engine_commit_char (lx_tn_engine, ibus_engine, new_char);

  return TRUE;
}

/*
vi:ts=2:nowrap:ai:expandtab
*/
