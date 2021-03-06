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

#include "lx-tham-im-table.h"
#include <wctype.h>

/*--------------------------*
 *  PRIVATE USE CHARACTERS  *
 *--------------------------*/

gboolean
lx_tham_is_pua (gunichar uc)
{
 return uc & 0x10000;
}

gint
lx_tham_get_pua_text (gunichar uc, gunichar *out_text, int out_text_nelm)
{
  switch (uc)
    {
      case LX_THAM_PUA_VOWEL_AM:
        if (out_text_nelm < 3)
          return 0;
        out_text[0] = 0x1a63;
        out_text[1] = 0x1a74;
        out_text[2] = 0;
        return 2;
    }

  return 0;
}

/*----------------------------*
 *  CHARACTER CLASSIFICATION  *
 *----------------------------*/

typedef enum _LxThamCharClass LxThamCharClass;

enum _LxThamCharClass
{
  XC_X,    /* Undefined */
  XC_LV,   /* Leading Vowels */
  XC_AV,   /* Above Vowels (I, II, UE, UEE, KONG, KAO HO NUENG) */
  XC_AD1,  /* Above Diacritics 1 (MAI SAT, MAI ANG LAN, FINAL NGA) */
  XC_AD2,  /* Above Diacritics 2 (NIKHAHIT) */
  XC_BV1,  /* Below Vowels 1 (U, UU) */
  XC_BV2,  /* Below Vowels 2 (AU BELOW) */
  XC_FV1,  /* Folowing Vowels 1 (A) */
  XC_FV2,  /* Folowing Vowels 2 (AA, TALL AA, AM) */
  XC_FV3,  /* Folowing Vowels 3 (OY) */
  XC_IV,   /* Independent Vowels (RU, LU, I, II, U, UU, E, O) */
  XC_C1,   /* Consonants 1 (Other Consonants) */
  XC_C2,   /* Consonants 2 (HIGH KXA, LOW KXA, LOW SA, HIGH FA, LOW FA,
              HIGH YA, RA, LLA, A, LOW HA, GREAT SA) */
  XC_PH,   /* Phinthu (SAKOT) */
  XC_S1,   /* Subjoins 1 (MEDIAL RA) */
  XC_S2,   /* Subjoins 2 (MEDIAL LA, LA TANG LAI) */
  XC_S3,   /* Subjoins 3 (DOUBLE DOT BELOW) */
  XC_S4,   /* Subjoins 4 (SCALED HIGH RATHA, MA, BA, SA) */
  XC_T,    /* Tones */
  XC_LG,   /* Ligatures (LAE) */
  XC_CR,   /* Cryptograms (CRYPTOGRAMMIC DOT) */
  XC_NP,   /* Numbers and Punctuation marks */

  XC_TOTAL, /* Character class count (not a class) */
};

static const LxThamCharClass char_class[144] =
{
  XC_C1 , XC_C1 , XC_C2 , XC_C1 , XC_C2 , XC_C1 , XC_C1 , XC_C1 , /* 0x1A20 */
  XC_C1 , XC_C1 , XC_C2 , XC_C1 , XC_C1 , XC_C1 , XC_C1 , XC_C1 , /* 0x1A28 */
  XC_C1 , XC_C1 , XC_C1 , XC_C1 , XC_C1 , XC_C1 , XC_C1 , XC_C1 , /* 0x1A30 */
  XC_C1 , XC_C1 , XC_C2 , XC_C1 , XC_C2 , XC_C1 , XC_C1 , XC_C1 , /* 0x1A38 */
  XC_C2 , XC_C2 , XC_IV , XC_C1 , XC_IV , XC_C1 , XC_C1 , XC_C1 , /* 0x1A40 */
  XC_C1 , XC_C1 , XC_C2 , XC_C2 , XC_C2 , XC_IV , XC_IV , XC_IV , /* 0x1A48 */
  XC_IV , XC_IV , XC_IV , XC_LG , XC_C2 , XC_S1 , XC_S2 , XC_S2 , /* 0x1A50 */
  XC_AD1, XC_AD1, XC_AD1, XC_S4 , XC_S4 , XC_S4 , XC_S4 , XC_X  , /* 0x1A58 */
  XC_PH , XC_FV1, XC_AD1, XC_FV2, XC_FV2, XC_AV , XC_AV , XC_AV , /* 0x1A60 */
  XC_AV , XC_BV1, XC_BV1, XC_AV , XC_BV2, XC_FV3, XC_LV , XC_LV , /* 0x1A68 */
  XC_LV , XC_LV , XC_LV , XC_AV , XC_AD2, XC_T  , XC_T  , XC_X  , /* 0x1A70 */
  XC_X  , XC_X  , XC_X  , XC_X  , XC_X  , XC_X  , XC_X  , XC_CR , /* 0x1A78 */
  XC_NP , XC_NP , XC_NP , XC_NP , XC_NP , XC_NP , XC_NP , XC_NP , /* 0x1A80 */
  XC_NP , XC_NP , XC_X  , XC_X  , XC_X  , XC_X  , XC_X  , XC_X  , /* 0x1A88 */
  XC_NP , XC_NP , XC_NP , XC_NP , XC_NP , XC_NP , XC_NP , XC_NP , /* 0x1A90 */
  XC_NP , XC_NP , XC_X  , XC_X  , XC_X  , XC_X  , XC_X  , XC_X  , /* 0x1A98 */
  XC_NP , XC_NP , XC_NP , XC_NP , XC_NP , XC_NP , XC_NP , XC_NP , /* 0x1AA0 */
  XC_NP , XC_NP , XC_NP , XC_NP , XC_NP , XC_NP , XC_X  , XC_X  , /* 0x1AA8 */
};

static LxThamCharClass
lx_tham_char_class (gunichar u)
{
  if (0x1a20 <= u && u <= 0x1aad)
    return char_class[u - 0x1a20];

  if (iswspace (u) || iswpunct (u) || iswdigit (u))
    return XC_NP;

  if (lx_tham_is_pua (u))
    {
      if (LX_THAM_PUA_VOWEL_AM == u)
        return XC_FV2;
    }

  /* DOUBLE DOT BELOW */
  if (0x0324 == u)
    return XC_S3;

  return XC_X;
}

/*-----------------------------*
 *  INPUT METHOD ACTION TABLE  *
 *-----------------------------*/

static LxThamImAction normal_action[XC_TOTAL][XC_TOTAL] =
{
#define P IA_P
#define A IA_A
#define W IA_W
#define R IA_R
#define S IA_S
#define C IA_C
       /* X, L, A, A, A, B, B, F, F, F, I, C, C, P, S, S, S, S, T, L, C, N */
       /*  , V, V, D, D, V, V, V, V, V, V, 1, 2, H, 1, 2, 3, 4,  , G, R, P */
       /*  ,  ,  , 1, 2, 1, 2, 1, 2, 3,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,   */
  /*X*/   R, P, R, R, R, R, R, R, R, R, A, A, A, R, R, R, R, R, R, A, R, A,
  /*LV*/  R, P, A, A, S, S, S, A, A, S, A, A, A, A, W, A, A, A, A, A, A, A,
  /*AV*/  R, P, R, R, A, R, A, S, A, S, A, A, A, A, R, A, A, A, A, A, A, A,
  /*AD1*/ R, P, R, R, R, R, R, S, S, S, A, A, A, A, R, A, A, A, A, A, A, A,
  /*AD2*/ R, P, R, R, R, R, R, S, A, S, A, A, A, S, R, R, R, R, A, A, R, A,
  /*BV1*/ R, P, S, A, A, R, R, S, S, S, A, A, A, S, A, R, R, R, A, A, R, A,
  /*BV2*/ R, P, R, A, R, R, R, A, R, R, R, A, A, S, R, R, R, R, A, A, R, A,
  /*FV1*/ R, P, R, R, R, R, R, R, R, R, A, A, A, R, R, R, R, R, R, A, R, A,
  /*FV2*/ R, P, R, A, R, R, R, A, R, R, A, A, A, A, R, A, A, A, R, A, A, A,
  /*FV3*/ R, P, R, R, R, R, R, R, R, R, A, A, A, R, R, R, R, R, R, A, R, A,
  /*IV*/  R, P, A, R, R, R, R, R, R, R, R, A, A, R, R, R, R, R, R, A, R, A,
  /*C1*/  R, P, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A,
  /*C2*/  R, P, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A,
  /*PH*/  R, P, R, R, R, R, R, R, R, R, R, A, R, R, R, R, R, R, R, R, R, R,
  /*S1*/  R, P, A, A, A, A, A, A, A, A, A, A, A, A, R, R, S, S, A, A, A, A,
  /*S2*/  R, P, A, A, A, A, S, A, A, A, A, A, A, A, R, R, S, S, A, A, A, A,
  /*S3*/  R, P, R, R, R, R, R, R, R, R, A, A, A, R, R, R, R, R, R, A, R, A,
  /*S4*/  R, P, A, A, A, A, S, A, A, A, A, A, A, A, A, S, R, R, A, A, A, A,
  /*T*/   R, P, R, R, R, R, R, A, A, A, A, A, A, A, R, R, A, A, R, A, A, A,
  /*LG*/  R, P, R, R, R, R, R, A, R, R, A, A, A, A, R, R, A, A, A, A, A, A,
  /*CR*/  R, P, A, A, A, A, A, A, A, A, A, A, A, S, A, A, R, A, A, A, A, A,
  /*NP*/  R, P, R, R, R, R, R, R, R, R, A, A, A, R, R, R, R, R, R, A, R, A,
#undef P
#undef A
#undef W
#undef R
#undef S
#undef C
};

LxThamImAction
lx_tham_im_normal_action (gunichar prev_c, gunichar input_c)
{
  return normal_action[lx_tham_char_class (prev_c)][lx_tham_char_class (input_c)];
}

LxThamImAction
lx_tham_im_preedit_action (gunichar prev_c, gunichar input_c)
{
  LxThamCharClass c0 = lx_tham_char_class (prev_c);
  LxThamCharClass c1 = lx_tham_char_class (input_c);

  return XC_LV == c0 && (XC_C1 == c1 || XC_C2 == c1 || XC_S4 == c1)
           ? IA_C : IA_R;
}

/*
vi:ts=2:nowrap:ai:expandtab
*/
