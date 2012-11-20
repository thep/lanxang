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
#include "lx-im-table.h"
#include <glib.h>
#include <string.h>

typedef struct _IBusLanXangEngine IBusLanXangEngine;
typedef struct _IBusLanXangEngineClass IBusLanXangEngineClass;

#define PREEDIT_BUFF_LEN 4
struct _IBusLanXangEngine
{
  IBusEngine parent;

  /* members here */
  gboolean is_preedit;
  gunichar preedit_str[PREEDIT_BUFF_LEN];
  gint     preedit_len;
  IscMode  isc_mode;
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
  lanxang_engine->is_preedit = FALSE;
  lanxang_engine->preedit_str[0] = 0;
  lanxang_engine->preedit_len = 0;
  lanxang_engine->isc_mode = ISC_BASIC;
}

static gboolean
is_client_support_surrounding (IBusEngine *engine)
{
  return engine->client_capabilities & IBUS_CAP_SURROUNDING_TEXT;
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
ibus_lanxang_engine_append_preedit (IBusLanXangEngine *lanxang_engine,
                                    gunichar new_char)
{
  if (lanxang_engine->preedit_len < PREEDIT_BUFF_LEN - 1)
    {
      lanxang_engine->preedit_str[lanxang_engine->preedit_len++] = new_char;
      lanxang_engine->preedit_str[lanxang_engine->preedit_len] = 0;
      return TRUE;
    }

  return FALSE;
}

static gunichar
ibus_lanxang_engine_get_prev_preedit_char (IBusLanXangEngine *lanxang_engine)
{
  if (lanxang_engine->preedit_len > 0)
    return lanxang_engine->preedit_str[lanxang_engine->preedit_len - 1];

  return 0;
}

static void
ibus_lanxang_engine_commit_preedit_reversed (IBusLanXangEngine *lanxang_engine)
{
  gunichar r_text[PREEDIT_BUFF_LEN];
  gint     i;
  IBusText *text;

  ibus_engine_hide_preedit_text (IBUS_ENGINE (lanxang_engine));

  /* prepare reversed text, clearing preedit string */
  for (i = 0; lanxang_engine->preedit_len > 0; lanxang_engine->preedit_len--)
    r_text[i++] = lanxang_engine->preedit_str[lanxang_engine->preedit_len - 1];
  r_text[i] = 0;

  /* commit it */
  text = ibus_text_new_from_ucs4 (r_text);
  ibus_engine_commit_text (IBUS_ENGINE (lanxang_engine), text);
}

static void
ibus_lanxang_engine_update_preedit (IBusLanXangEngine *lanxang_engine)
{
  IBusText *text;

  text = ibus_text_new_from_ucs4 (lanxang_engine->preedit_str);
  ibus_text_append_attribute(text,
                             IBUS_ATTR_TYPE_UNDERLINE,
                             IBUS_ATTR_UNDERLINE_SINGLE,
                             0, -1);

  ibus_engine_update_preedit_text (IBUS_ENGINE (lanxang_engine),
                                   text, lanxang_engine->preedit_len, TRUE);
}

static gunichar
ibus_lanxang_engine_get_prev_surrounding_char (IBusLanXangEngine *lanxang_engine)
{
  if (is_client_support_surrounding (IBUS_ENGINE (lanxang_engine)))
    {
      IBusText *surrounding;
      guint     cursor_pos;
      guint     anchor_pos;
      gunichar *u_surrounding;
      gunichar  ret = 0;

      ibus_engine_get_surrounding_text (IBUS_ENGINE (lanxang_engine),
                                        &surrounding, &cursor_pos, &anchor_pos);
      u_surrounding = g_utf8_to_ucs4 (ibus_text_get_text (surrounding), -1,
                                      NULL, NULL, NULL);
      if (u_surrounding)
        {
          ret = (cursor_pos > 0) ? u_surrounding[cursor_pos - 1] : 0;
          g_free (u_surrounding);
        }

      return ret;
    }

  return 0;
}

static gboolean
ibus_lanxang_engine_commit_char_swapped (IBusLanXangEngine *lanxang_engine,
                                         gunichar new_char)
{
  gunichar prev_char;
  gunichar commit_buff[3];
  IBusText *commit_text;

  if (is_client_support_surrounding (IBUS_ENGINE (lanxang_engine)))
    {
      prev_char = ibus_lanxang_engine_get_prev_surrounding_char (lanxang_engine);
      ibus_engine_delete_surrounding_text (IBUS_ENGINE (lanxang_engine), -1, 1);

      commit_buff[0] = new_char;
      commit_buff[1] = prev_char;
      commit_buff[2] = 0;
      commit_text = ibus_text_new_from_ucs4 (commit_buff);

      ibus_engine_commit_text (IBUS_ENGINE (lanxang_engine), commit_text);

      return TRUE;
    }

  return FALSE;
}

static gboolean
ibus_lanxang_engine_process_key_event (IBusEngine *engine,
                                       guint       keyval,
                                       guint       keycode,
                                       guint       modifiers)
{
  IBusLanXangEngine *lanxang_engine = IBUS_LANXANG_ENGINE (engine);
  gunichar new_char, prev_char;
  LxImAction action;
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
  if (0 == new_char)
    return FALSE;

  if (lanxang_engine->is_preedit)
    {
      prev_char = ibus_lanxang_engine_get_prev_preedit_char (lanxang_engine);
      action = lx_im_preedit_action (prev_char, new_char);
      if (IA_C == action)
        {
          ibus_lanxang_engine_append_preedit (lanxang_engine, new_char);
          ibus_lanxang_engine_commit_preedit_reversed (lanxang_engine);
          lanxang_engine->is_preedit = FALSE;
        }
    }
  else
    {
      prev_char = ibus_lanxang_engine_get_prev_surrounding_char (lanxang_engine);
      action = lx_im_normal_action (prev_char, new_char);

      switch (lanxang_engine->isc_mode)
        {
          case ISC_PASSTHROUGH:
            if (IA_R == action || IA_S == action)
              action = IA_A;
            break;
          case ISC_STRICT:
            if (IA_S == action)
              action = IA_R;
            break;
          case ISC_BASIC:
          default:
            if (IA_S == action)
              action = IA_A;
            break;
        }

      switch (action)
        {
          case IA_P:
            ibus_lanxang_engine_append_preedit (lanxang_engine, new_char);
            ibus_lanxang_engine_update_preedit (lanxang_engine);
            lanxang_engine->is_preedit = TRUE;
            break;

          case IA_A:
            text = ibus_text_new_from_unichar (new_char);
            ibus_engine_commit_text (engine, text);
            break;

          case IA_W:
            ibus_lanxang_engine_commit_char_swapped (lanxang_engine, new_char);
            break;

          case IA_R:
          case IA_S:
            goto reject_char;

          case IA_C:
          default:
            /* shouldn't reach here! */
            break;
        }
    }

  return TRUE;

reject_char:
  /* gdk_beep() */
  return TRUE;
}

/*
vi:ts=2:nowrap:ai:expandtab
*/
