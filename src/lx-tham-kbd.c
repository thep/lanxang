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

#include "lx-tham-kbd.h"
#include "lx-tham-im-table.h"

/* Tham Keyboard Layout */
static const gunichar tham_qwerty_map[] =
{
  0x0020, 0x1a64, 0x1a59, 0x1a92, 0x1a93, 0x1a94, 0x1a58, 0x1a26,
  0x1a96, 0x1a97, 0x1a95, 0x1a99, 0x1a3e, 0x1a21, 0x1a72, 0x1a3a,
  0x1a27, 0x1a40, 0x1a6d, 0x1a6c, 0x1a3d, 0x1a33, 0x1a69, 0x1a67,
  0x1a23, 0x1a32, 0x1a2a, 0x1a45, 0x1a30, 0x1a29, 0x1a4a, 0x1a44,
  0x1a91, 0x1a42, 0x1a60, 0x1a28, 0x1a2d, 0x1a4f, 0x1a70, 0x1a2b,
  0x1a6b, 0x1a31, 0x1a4e, 0x1a47, 0x1a46, 0x1a50, 0x1a7a, 0x1aaa,
  0x1a2c, 0x1a90, 0x1a2f, 0x1a25, 0x1a35, 0x1a4d, 0x1a4c, 0x1a54,
  0x1a51, 0x1a74, 0x1a55, 0x1a37, 0x1a22, 0x1a43, 0x1a6a, 0x1a98,
  0x1aa5, 0x1a3c, 0x1a65, 0x1a6f, 0x1a20, LX_THAM_PUA_VOWEL_AM, 0x1a2f, 0x1a6e,
  0x1a76, 0x1a41, 0x1a75, 0x1a63, 0x1a48, 0x1a34, 0x1a68, 0x1a36,
  0x1a3f, 0x1aa7, 0x1a3b, 0x1a49, 0x1a61, 0x1a66, 0x1a4b, 0x1a71,
  0x1a38, 0x1a62, 0x1a39, 0x1a2e, 0x1a24, 0x1a56, 0x1aac, 0x007f
};
static const gunichar tham_qwerty_map_lv3[] =
{
  0x0020, 0x1a64, 0x1a59, 0x1a92, 0x1a93, 0x1a94, 0x1a58, 0x1a26,
  0x1a96, 0x1a97, 0x1a95, 0x1a99, 0x1a5c, 0x1a21, 0x1a72, 0x1a3a,
  0x1a27, 0x1a40, 0x1a6d, 0x1a6c, 0x1a3d, 0x1a33, 0x1a69, 0x1a67,
  0x1a23, 0x1a32, 0x1a2a, 0x1a45, 0x1a30, 0x1a29, 0x1a4a, 0x1a44,
  0x1a91, 0x1a42, 0x1a60, 0x1a28, 0x1a2d, 0x1a4f, 0x1a70, 0x1a2b,
  0x1a6b, 0x1a31, 0x1a4e, 0x1a47, 0x1a46, 0x1a50, 0x1a7a, 0x1aaa,
  0x1a2c, 0x1a90, 0x1a2f, 0x1a25, 0x1a35, 0x1a4d, 0x1a4c, 0x1a54,
  0x1a51, 0x1a74, 0x1a55, 0x1a5d, 0x1a22, 0x1a53, 0x1a6a, 0x1a98,
  0x1aa5, 0x1a3c, 0x1a65, 0x1a6f, 0x0324, LX_THAM_PUA_VOWEL_AM, 0x1a52, 0x1a6e,
  0x1a76, 0x1a41, 0x1a75, 0x1a63, 0x1a5e, 0x1a34, 0x1a68, 0x1aab,
  0x1a3f, 0x1aa7, 0x1a5a, 0x1a49, 0x1a61, 0x1a66, 0x1a4b, 0x1a71,
  0x1a38, 0x1a62, 0x1a39, 0x1a2e, 0x1a24, 0x1a56, 0x1aac, 0x007f
};

gunichar
lx_tham_map_key (guint keyval, gboolean is_lv3)
{
  if (0x20 <= keyval && keyval <= 0x7f)
    {
      /* English -> Tham */
      return is_lv3 ? tham_qwerty_map_lv3[keyval - 0x20]
                    : tham_qwerty_map[keyval - 0x20];
    }
  else if (0x1001a20 <= keyval && keyval <= 0x1001aad)
    {
      /* XKB Tham Unicode keysym */
      return keyval - 0x1000000;
    }

  return 0;
}

#define N_KEYCODES  54

static const gunichar tham_keycode_map[3][N_KEYCODES] =
{
  { /* level 1 */
    0x0000, 0x0000, 0x1a40, 0x1a6d, 0x1a6c, 0x1a3d, 0x1a33, 0x1a69, /* 00 */
    0x1a67, 0x1a23, 0x1a32, 0x1a27, 0x1a21, 0x1a29, 0x0008, 0x0009,
    0x1aa7, 0x1a71, LX_THAM_PUA_VOWEL_AM, 0x1a3b, 0x1a61, 0x1a62, 0x1a66, 0x1a41, /* 10 */
    0x1a36, 0x1a3f, 0x1a37, 0x1a43, 0x000d, 0x0000, 0x1a3c, 0x1a49,
    0x1a20, 0x1a2f, 0x1a6e, 0x1a76, 0x1a75, 0x1a63, 0x1a48, 0x1a45, /* 20 */
    0x1a26, 0x1aa5, 0x0000, 0x1a22, 0x1a39, 0x1a38, 0x1a6f, 0x1a4b,
    0x1a65, 0x1a68, 0x1a34, 0x1a3e, 0x1a72, 0x1a3a,                 /* 30 */
  },
  { /* level 2 */
    0x0000, 0x0000, 0x1a64, 0x1a91, 0x1a92, 0x1a93, 0x1a94, 0x1a6a, /* 00 */
    0x1a58, 0x1a95, 0x1a96, 0x1a97, 0x1a98, 0x1a99, 0x0008, 0x0009,
    0x1a90, 0x1a54, 0x1a4f, 0x1a2f, 0x1a35, 0x1a74, 0x1a4d, 0x1a31, /* 10 */
    0x1aaa, 0x1a2c, 0x1a2e, 0x1a56, 0x000d, 0x0000, 0x1a42, 0x1a25,
    0x1a2d, 0x1a70, 0x1a2b, 0x1a6b, 0x1a4e, 0x1a47, 0x1a46, 0x1a2a, /* 20 */
    0x1a59, 0x1aac, 0x0000, 0x1a24, 0x1a55, 0x1a51, 0x1a28, 0x1a4c,
    0x1a60, 0x1a7a, 0x1a50, 0x1a30, 0x1a4a, 0x1a44,                 /* 30 */
  },
  { /* level 3 */
    0x0000, 0x0000, 0x1a40, 0x1a6d, 0x1a6c, 0x1a3d, 0x1a33, 0x1a69, /* 00 */
    0x1a67, 0x1a23, 0x1a32, 0x1a27, 0x1a21, 0x1a29, 0x0008, 0x0009,
    0x1aa7, 0x1a71, 0x1a73, 0x1a5a, 0x1a61, 0x1a62, 0x1a66, 0x1a41, /* 10 */
    0x1aab, 0x1a3f, 0x1a5d, 0x1a53, 0x000d, 0x0000, 0x1a3c, 0x1a49,
    0x0324, 0x1a52, 0x1a6e, 0x1a76, 0x1a75, 0x1a63, 0x1a5e, 0x1a45, /* 20 */
    0x1a26, 0x1aa5, 0x0000, 0x1a22, 0x1a39, 0x1a38, 0x1a6f, 0x1a4b,
    0x1a65, 0x1a68, 0x1a34, 0x1a5c, 0x1a72, 0x1a3a,                 /* 30 */
  },
};

gunichar lx_tham_map_keycode (guint keycode, gint shift_lv)
{
  if (shift_lv >= 3 || keycode >= N_KEYCODES)
    return 0;

  return tham_keycode_map[shift_lv][keycode];
}

/*
vi:ts=2:nowrap:ai:expandtab
*/
