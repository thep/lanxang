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

#include "lx-pali-im-table.h"
#include <string.h>

/*----------------------------*
 *  CHARACTER CLASSIFICATION  *
 *----------------------------*/

typedef enum _LxPaliCharClass LxPaliCharClass;

enum _LxPaliCharClass
{
  NC_X,     /* non composibles */
  NC_C,     /* consonants */
  NC_LV,    /* leading vowels */
  NC_FV1,   /* following vowels 1 */
  NC_FV2,   /* following vowels 2 */
  NC_FV3,   /* following vowels 3 */
  NC_BV1,   /* below vowels 1 */
  NC_BV2,   /* below vowels 2 */
  NC_BD,    /* below diacritics */
  NC_T,     /* tonemarks */
  NC_AD1,   /* above diacritics 1 */
  NC_AD2,   /* above diacritics 2 */
  NC_AD3,   /* above diacritics 3 */
  NC_AV1,   /* above vowels 1 */
  NC_AV2,   /* above vowels 2 */
  NC_AV3,   /* above vowels 3 */
  NC_TOTAL, /* Character class count (not a class) */
};

static const LxPaliCharClass char_class[128] =
{
  NC_X,   NC_C,   NC_C,   NC_C,   NC_C,   NC_C,   NC_C,   NC_C,   /* 0E80 */
  NC_C,   NC_C,   NC_C,   NC_C,   NC_C,   NC_C,   NC_C,   NC_C,   /* 0E88 */
  NC_C,   NC_C,   NC_C,   NC_C,   NC_C,   NC_C,   NC_C,   NC_C,   /* 0E90 */
  NC_C,   NC_C,   NC_C,   NC_C,   NC_C,   NC_C,   NC_C,   NC_C,   /* 0E98 */
  NC_C,   NC_C,   NC_C,   NC_C,   NC_FV3, NC_C,   NC_FV3, NC_C,   /* 0EA0 */
  NC_C,   NC_C,   NC_C,   NC_C,   NC_C,   NC_C,   NC_C,   NC_X,   /* 0EA8 */
  NC_FV1, NC_AV2, NC_FV2, NC_FV1, NC_AV1, NC_AV3, NC_AV2, NC_AV3, /* 0EB0 */
  NC_BV1, NC_BV2, NC_BD,  NC_AV2, NC_C,   NC_FV1, NC_X,   NC_X,   /* 0EB8 */
  NC_LV,  NC_LV,  NC_LV,  NC_LV,  NC_LV,  NC_X,   NC_X,   NC_X,   /* 0EC0 */
  NC_T,   NC_T,   NC_T,   NC_T,   NC_AD1, NC_AD1, NC_X,   NC_X,   /* 0EC8 */
  NC_X,   NC_X,   NC_X,   NC_X,   NC_X,   NC_X,   NC_X,   NC_X,   /* 0ED0 */
  NC_X,   NC_X,   NC_X,   NC_X,   NC_C,   NC_C,   NC_C,   NC_C,   /* 0ED8 */
  NC_X,   NC_X,   NC_X,   NC_X,   NC_X,   NC_X,   NC_X,   NC_X,   /* 0EE0 */
  NC_X,   NC_X,   NC_X,   NC_X,   NC_X,   NC_X,   NC_X,   NC_X,   /* 0EE8 */
  NC_X,   NC_X,   NC_X,   NC_X,   NC_X,   NC_X,   NC_X,   NC_X,   /* 0EF0 */
  NC_X,   NC_X,   NC_X,   NC_X,   NC_X,   NC_X,   NC_X,   NC_X,   /* 0EF8 */
};

static LxPaliCharClass
lx_pali_char_class (gunichar u)
{
  if (0x0e80 <= u && u <= 0x0eff)
    return char_class[u - 0x0e80];

  return NC_X;
}

/*-----------------------------*
 *  INPUT METHOD ACTION TABLE  *
 *-----------------------------*/

static LxPaliImAction normal_action[NC_TOTAL][NC_TOTAL] =
{
#define A NA_A
#define R NA_R
#define S NA_S
/* X  C  L  F  F  F  B  B  B  T  A  A  A  A  A  A */
/*       V  V  V  V  V  V  D     D  D  D  V  V  V */
/*          1  2  3  1  2        1  2  3  1  2  3 */
  {A, A, A, S, S, A, R, R, R, R, R, R, R, R, R, R}, /*X*/
  {A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A}, /*C*/
  {S, A, S, S, S, S, R, R, R, R, R, R, R, R, R, R}, /*LV*/
  {A, A, A, A, S, A, R, R, R, R, R, R, R, R, R, R}, /*FV1*/
  {A, A, A, A, S, A, R, R, A, R, R, R, R, R, R, R}, /*FV2*/
  {A, A, A, S, A, S, R, R, R, R, R, R, R, R, R, R}, /*FV3*/
  {A, A, A, A, S, A, R, R, A, A, A, R, R, R, R, R}, /*BV1*/
  {A, A, A, S, S, A, R, R, A, A, R, R, R, R, R, R}, /*BV2*/
  {A, A, A, S, S, A, R, R, R, R, R, R, R, R, R, R}, /*BD*/
  {A, A, A, A, A, A, R, R, A, R, R, R, R, R, R, R}, /*T*/
  {A, A, A, S, S, A, R, R, R, A, R, R, R, R, R, R}, /*AD1*/
  {A, A, A, S, S, A, R, R, R, R, R, R, R, R, R, R}, /*AD2*/
  {A, A, A, S, S, A, R, R, R, R, R, R, R, R, R, R}, /*AD3*/
  {A, A, A, S, S, A, R, R, A, A, A, R, R, R, R, R}, /*AV1*/
  {A, A, A, S, S, A, R, R, A, A, R, R, R, R, R, R}, /*AV2*/
  {A, A, A, S, S, A, R, R, A, A, R, A, R, R, R, R}, /*AV3*/
#undef A
#undef R
#undef S
};

LxPaliImAction
lx_pali_im_action (gunichar prev_c, gunichar input_c)
{
  return normal_action[lx_pali_char_class (prev_c)][lx_pali_char_class (input_c)];
}

/*
vi:ts=2:nowrap:ai:expandtab
*/
