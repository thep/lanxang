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
#include "lx-tham-engine.h"
#include "lx-tham-kbd.h"
#include "lx-tham-im-table.h"
#include "ibus-config.h"
#include <glib.h>
#include <string.h>

#define N_ELM(ar)  (sizeof(ar)/sizeof((ar)[0]))

typedef struct _LxThamEngine LxThamEngine;
typedef struct _LxThamEngineClass LxThamEngineClass;

#define PREEDIT_BUFF_LEN 4
struct _LxThamEngine
{
  GObject  parent;

  /* members here */
  gboolean is_preedit;
  gunichar preedit_str[PREEDIT_BUFF_LEN];
  gint     preedit_len;
  IscMode  isc_mode;
};

struct _LxThamEngineClass
{
  GObjectClass parent;
};

static void lx_iengine_interface_init (LxIEngineInterface *iface);
static gboolean lx_tham_engine_process_key_event (LxIEngine  *lx_iengine,
                                                  IBusEngine *engine,
                                                  guint       keyval,
                                                  guint       keycode,
                                                  guint       modifiers);


G_DEFINE_TYPE_WITH_CODE (LxThamEngine, lx_tham_engine, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (LX_TYPE_IENGINE,
                                                lx_iengine_interface_init));

static void
lx_iengine_interface_init (LxIEngineInterface *iface)
{
  iface->process_key_event = lx_tham_engine_process_key_event;
}

static void
lx_tham_engine_class_init (LxThamEngineClass *klass)
{
}

static void
lx_tham_engine_init (LxThamEngine *lx_tham_engine)
{
  IBusLanXangSetupOptions opt;

  lx_tham_engine->is_preedit = FALSE;
  lx_tham_engine->preedit_str[0] = 0;
  lx_tham_engine->preedit_len = 0;

  /* Read config */
  ibus_lanxang_read_config (&opt);
  lx_tham_engine->isc_mode = opt.tham_isc_mode;
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
lx_tham_engine_append_preedit (LxThamEngine *lx_tham_engine,
                               gunichar new_char)
{
  if (lx_tham_is_pua (new_char))
    {
      gunichar new_text[8];
      gint     len, i;

      len = lx_tham_get_pua_text (new_char, new_text, N_ELM (new_text));
      if (0 == len)
        return FALSE;

      if (lx_tham_engine->preedit_len < PREEDIT_BUFF_LEN - len)
        {
          for (i = 0; i < len; i++)
            {
              lx_tham_engine->preedit_str[lx_tham_engine->preedit_len++]
                = new_text[i];
            }
          lx_tham_engine->preedit_str[lx_tham_engine->preedit_len] = 0;
          return TRUE;
        }
    }
  else
    {
      if (lx_tham_engine->preedit_len < PREEDIT_BUFF_LEN - 1)
        {
          lx_tham_engine->preedit_str[lx_tham_engine->preedit_len++] = new_char;
          lx_tham_engine->preedit_str[lx_tham_engine->preedit_len] = 0;
          return TRUE;
        }
    }

  return FALSE;
}

/* Return TRUE if preedit text becomes empty */
static gboolean
lx_tham_engine_preedit_cut_last (LxThamEngine *lx_tham_engine)
{
  if (lx_tham_engine->preedit_len > 0)
    {
      lx_tham_engine->preedit_str[--lx_tham_engine->preedit_len] = 0;
      return lx_tham_engine->preedit_len == 0;
    }

  return TRUE;
}

static void
lx_tham_engine_preedit_clear (LxThamEngine *lx_tham_engine)
{
  lx_tham_engine->preedit_str[0] = 0;
  lx_tham_engine->preedit_len = 0;
}

static gunichar
lx_tham_engine_get_prev_preedit_char (LxThamEngine *lx_tham_engine)
{
  if (lx_tham_engine->preedit_len > 0)
    return lx_tham_engine->preedit_str[lx_tham_engine->preedit_len - 1];

  return 0;
}

static void
lx_tham_engine_commit_preedit_reversed (LxThamEngine *lx_tham_engine,
                                        IBusEngine *ibus_engine)
{
  gunichar r_text[PREEDIT_BUFF_LEN];
  gint     i;
  IBusText *text;

  ibus_engine_hide_preedit_text (ibus_engine);

  /* prepare reversed text, clearing preedit string */
  for (i = 0; lx_tham_engine->preedit_len > 0; lx_tham_engine->preedit_len--)
    r_text[i++] = lx_tham_engine->preedit_str[lx_tham_engine->preedit_len - 1];
  r_text[i] = 0;

  /* commit it */
  text = ibus_text_new_from_ucs4 (r_text);
  ibus_engine_commit_text (ibus_engine, text);
}

static void
lx_tham_engine_update_preedit (LxThamEngine *lx_tham_engine,
                               IBusEngine   *ibus_engine)
{
  IBusText *text;

  text = ibus_text_new_from_ucs4 (lx_tham_engine->preedit_str);
  ibus_text_append_attribute(text,
                             IBUS_ATTR_TYPE_UNDERLINE,
                             IBUS_ATTR_UNDERLINE_SINGLE,
                             0, -1);

  ibus_engine_update_preedit_text (ibus_engine,
                                   text, lx_tham_engine->preedit_len, TRUE);
}

static gunichar
lx_tham_engine_get_prev_surrounding_char (LxThamEngine *lx_tham_engine,
                                          IBusEngine   *ibus_engine)
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
lx_tham_engine_commit_char (LxThamEngine *lx_tham_engine,
                            IBusEngine   *ibus_engine,
                            gunichar      new_char)
{
  IBusText *text;

  /* Private-Use-Area char? */
  if (lx_tham_is_pua (new_char))
    {
      gunichar new_text[8];
      if (!lx_tham_get_pua_text (new_char, new_text, N_ELM (new_text)))
        return FALSE;
      text = ibus_text_new_from_ucs4 (new_text);
    }
  else
    {
      text = ibus_text_new_from_unichar (new_char);
    }

  ibus_engine_commit_text (ibus_engine, text);

  return TRUE;
}

static gboolean
lx_tham_engine_commit_char_swapped (LxThamEngine *lx_tham_engine,
                                    IBusEngine   *ibus_engine,
                                    gunichar      new_char)
{
  gunichar prev_char;
  gunichar commit_buff[10];
  IBusText *commit_text;
  gint     len;

  if (is_client_support_surrounding (ibus_engine))
    {
      prev_char = lx_tham_engine_get_prev_surrounding_char (lx_tham_engine,
                                                            ibus_engine);
      ibus_engine_delete_surrounding_text (ibus_engine, -1, 1);

      if (lx_tham_is_pua (new_char))
        {
          len = lx_tham_get_pua_text (new_char, commit_buff, N_ELM (commit_buff));
        }
      else
        {
          commit_buff[0] = new_char;
          len = 1;
        }
      commit_buff[len++] = prev_char;
      commit_buff[len] = 0;
      commit_text = ibus_text_new_from_ucs4 (commit_buff);

      ibus_engine_commit_text (ibus_engine, commit_text);

      return TRUE;
    }

  return FALSE;
}

/* Return TRUE if key is handled */
static gboolean
lx_tham_engine_process_preedit_keys (LxThamEngine *lx_tham_engine,
                                     IBusEngine   *ibus_engine,
                                     guint         keyval,
                                     guint         modifiers)
{
  if (IBUS_BackSpace == keyval)
    {
      if (lx_tham_engine_preedit_cut_last (lx_tham_engine))
        {
          ibus_engine_hide_preedit_text (ibus_engine);
          lx_tham_engine->is_preedit = FALSE;
        }
      else
        {
          lx_tham_engine_update_preedit (lx_tham_engine, ibus_engine);
        }
      return TRUE;
    }

  if (is_context_lost_key (keyval))
    {
      lx_tham_engine_preedit_clear (lx_tham_engine);
      ibus_engine_hide_preedit_text (ibus_engine);
      lx_tham_engine->is_preedit = FALSE;
      return FALSE;
    }

  return FALSE;
}

static gboolean
lx_tham_engine_process_key_event (LxIEngine  *lx_iengine,
                                  IBusEngine *ibus_engine,
                                  guint       keyval,
                                  guint       keycode,
                                  guint       modifiers)
{
  LxThamEngine *lx_tham_engine = LX_THAM_ENGINE (lx_iengine);
  gint shift_lv;
  gunichar new_char, prev_char;
  LxThamImAction action;

  if (modifiers & IBUS_RELEASE_MASK)
    return FALSE;

  if (modifiers & (IBUS_CONTROL_MASK | IBUS_MOD1_MASK))
    return FALSE;

  if (lx_tham_engine->is_preedit)
    {
      /* process editor keys */
      if (lx_tham_engine_process_preedit_keys (lx_tham_engine, ibus_engine,
                                               keyval, modifiers))
        {
          return TRUE;
        }
    }

  if (is_context_lost_key (keyval))
    return FALSE;

  if (0 == keyval || is_context_intact_key (keyval))
    return FALSE;

  shift_lv = !(modifiers & (IBUS_SHIFT_MASK | IBUS_MOD5_MASK)) ? 0
               : ((modifiers & IBUS_MOD5_MASK) ? 2 : 1);
  new_char = lx_tham_map_keycode (keycode, shift_lv);
  if (0 == new_char)
    return FALSE;

  if (lx_tham_engine->is_preedit)
    {
      prev_char = lx_tham_engine_get_prev_preedit_char (lx_tham_engine);
      action = lx_tham_im_preedit_action (prev_char, new_char);
      if (IA_C == action)
        {
          lx_tham_engine_append_preedit (lx_tham_engine, new_char);
          lx_tham_engine_commit_preedit_reversed (lx_tham_engine, ibus_engine);
          lx_tham_engine->is_preedit = FALSE;
        }
    }
  else
    {
      prev_char = lx_tham_engine_get_prev_surrounding_char (lx_tham_engine,
                                                            ibus_engine);
      action = lx_tham_im_normal_action (prev_char, new_char);

      switch (lx_tham_engine->isc_mode)
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
            lx_tham_engine_append_preedit (lx_tham_engine, new_char);
            lx_tham_engine_update_preedit (lx_tham_engine, ibus_engine);
            lx_tham_engine->is_preedit = TRUE;
            break;

          case IA_A:
            if (!lx_tham_engine_commit_char (lx_tham_engine, ibus_engine,
                                             new_char))
              goto reject_char;
            break;

          case IA_W:
            lx_tham_engine_commit_char_swapped (lx_tham_engine, ibus_engine,
                                                new_char);
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
