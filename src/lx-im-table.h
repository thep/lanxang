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

#ifndef __LX_IM_TABLE_H__
#define __LX_IM_TABLE_H__

#include <glib.h>

#define LX_PUA_VOWEL_AM  0x10001

/* PUA internal use */
gboolean lx_is_pua (gunichar uc);
/* returns out text len */
gint lx_get_pua_text (gunichar uc, gunichar *out_text, int out_text_nelm);

typedef enum _LxImAction LxImAction;

/*
 * IM actions
 */
enum _LxImAction
{
  IA_P,  /* Enter pre-edit mode */
  IA_A,  /* Accept the input character */
  IA_W,  /* Accept the input character and swap it with previous character */
  IA_R,  /* Reject the input character */
  IA_S,  /* Reject the input character in strict mode, accept it otherwise */
  IA_C,  /* Commit the pre-edit string */
};

LxImAction lx_im_normal_action (gunichar prev_c, gunichar input_c);
LxImAction lx_im_preedit_action (gunichar prev_c, gunichar input_c);

#endif /* __LX_IM_TABLE_H__ */

/*
vi:ts=2:nowrap:ai:expandtab
*/
