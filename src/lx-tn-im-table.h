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

#ifndef __LX_TN_IM_TABLE_H__
#define __LX_TN_IM_TABLE_H__

#include <glib.h>

typedef enum _LxTNImAction LxTNImAction;

/*
 * IM actions
 */
enum _LxTNImAction
{
  NA_A,  /* Accept the input character */
  NA_R,  /* Reject the input character */
  NA_S,  /* Reject the input character in strict mode, accept it otherwise */
};

LxTNImAction lx_tn_im_action (gunichar prev_c, gunichar input_c);

/*
 * Surround text conversion
 */

/* Virama */
#define TN_PHINTHU    0x0EBA

/* Subjoins */
#define TN_SUBBAIMAI  0x0EE0
#define TN_SUBMA      0x0EE1
#define TN_SUBLING    0x0EE2
#define TN_SUBSUA     0x0EE3
#define TN_SUBPHAN    0x0EE4
#define TN_SUBTHONG   0x0EE5
#define TN_SUBDEK     0x0EE6
#define TN_SUBTAO     0x0EE7
#define TN_SUBCHAN    0x0EE8
#define TN_SUBNU      0x0EE9

/* Conjuncts */
#define TN_KHAINU     0x0EF0
#define TN_KHAIMA     0x0EF1
#define TN_KHWAINU    0x0EF2
#define TN_KHWAIMA    0x0EF3
#define TN_THUNGNU    0x0EF4
#define TN_THUNGLING  0x0EF5
#define TN_SUANU      0x0EF6
#define TN_SUAMA      0x0EF7

typedef struct _LxTNConv LxTNConv;

struct _LxTNConv
{
  gint   del_offset;
  gchar  commit_text[8];
};

gboolean lx_tn_im_conversion (const gchar *surrounding,
                              gint         cursor_pos,
                              gint         anchor_pos,
                              gunichar     input_char,
                              LxTNConv    *conv);

#endif /* __LX_TN_IM_TABLE_H__ */

/*
vi:ts=2:nowrap:ai:expandtab
*/
