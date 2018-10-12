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

#ifndef __LX_PALI_IM_TABLE_H__
#define __LX_PALI_IM_TABLE_H__

#include <glib.h>

typedef enum _LxPaliImAction LxPaliImAction;

/*
 * IM actions
 */
enum _LxPaliImAction
{
  NA_A,  /* Accept the input character */
  NA_R,  /* Reject the input character */
  NA_S,  /* Reject the input character in strict mode, accept it otherwise */
};

LxPaliImAction lx_pali_im_action (gunichar prev_c, gunichar input_c);

#endif /* __LX_PALI_IM_TABLE_H__ */

/*
vi:ts=2:nowrap:ai:expandtab
*/
