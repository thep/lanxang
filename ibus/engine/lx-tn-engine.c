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
#include "lx-tn-im-table.h"
#include "ibus-config.h"
#include "im-utils.h"
#include <glib.h>
#include <string.h>

#define N_ELM(ar)  (sizeof(ar)/sizeof((ar)[0]))

typedef struct _LxTNEngine LxTNEngine;
typedef struct _LxTNEngineClass LxTNEngineClass;

#define PREEDIT_BUFF_LEN 4
struct _LxTNEngine
{
  GObject  parent;

  /* members here */
  IscMode  isc_mode;
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
  IBusLanXangSetupOptions opt;

  /* Read config */
  ibus_lanxang_read_config (&opt);
  lx_tn_engine->isc_mode = opt.tham_isc_mode;
}

static gunichar
lx_tn_engine_get_prev_surrounding_char (LxTNEngine *lx_tn_engine,
                                        IBusEngine *ibus_engine)
{
  if (is_client_support_surrounding (ibus_engine))
    {
      IBusText *surrounding;
      guint     cursor_pos;
      guint     anchor_pos;
      gunichar *u_surrounding;
      gunichar  ret = 0;

      ibus_engine_get_surrounding_text (ibus_engine,
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
lx_tn_engine_convert_seq (LxTNEngine *lx_tn_engine,
                          IBusEngine *ibus_engine,
                          gunichar    input_char)
{
  if (is_client_support_surrounding (ibus_engine))
    {
      IBusText *surrounding;
      guint     cursor_pos;
      guint     anchor_pos;
      LxTNConv  conv;

      ibus_engine_get_surrounding_text (ibus_engine,
                                        &surrounding, &cursor_pos, &anchor_pos);
      if (lx_tn_im_conversion (ibus_text_get_text (surrounding),
                               cursor_pos, anchor_pos, input_char, &conv))
        {
          IBusText *text;
          ibus_engine_delete_surrounding_text (ibus_engine,
                                               conv.del_offset,
                                               -conv.del_offset);
          text = ibus_text_new_from_static_string (conv.commit_text);
          ibus_engine_commit_text (ibus_engine, text);

          return TRUE;
        }
    }

  return FALSE;
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
  gunichar new_char, prev_char;
  LxTNImAction action;

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

  if (lx_tn_engine_convert_seq (lx_tn_engine, ibus_engine, new_char))
    return TRUE;

  prev_char = lx_tn_engine_get_prev_surrounding_char (lx_tn_engine,
                                                      ibus_engine);
  action = lx_tn_im_action (prev_char, new_char);

  switch (lx_tn_engine->isc_mode)
    {
      case ISC_PASSTHROUGH:
        if (NA_R == action || NA_S == action)
          action = NA_A;
        break;
      case ISC_STRICT:
        if (NA_S == action)
          action = NA_R;
        break;
      case ISC_BASIC:
      default:
        if (NA_S == action)
          action = NA_A;
        break;
    }

  switch (action)
    {
      case NA_A:
        if (!lx_tn_engine_commit_char (lx_tn_engine, ibus_engine, new_char))
          goto reject_char;
        break;

      case NA_R:
      case NA_S:
        goto reject_char;

      default:
        /* shouldn't reach here! */
        break;
    }

  return TRUE;

reject_char:
  /* gdk_beep() */
  return TRUE;
}

/*
vi:ts=2:nowrap:ai:expandtab
*/
