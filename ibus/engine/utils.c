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

#include <ibus.h>

#define LANXANG_XML  COMPONENTDIR "/lanxang.xml"

IBusComponent *
ibus_lanxang_get_component ()
{
  IBusComponent *component;

  component = ibus_component_new_from_file (LANXANG_XML);
  if (!component)
    {
      /* fallback to manually creation */
      component = ibus_component_new ("org.freedesktop.IBus.LanXang",
                                      "LanXang Component",
                                      VERSION,
                                      "GPL",
                                      "Theppitak Karoonboonyanan <thep@linux.thai.net>",
                                      "http://github.com/thep/lanxang",
                                      "",
                                      "ibus-lanxang");
      ibus_component_add_engine (component,
                                 ibus_engine_desc_new ("lanxang-tham",
                                                       "LanXang Tham",
                                                       "Input method for Lao/Esaan Tham script",
                                                       "lana",
                                                       "GPL",
                                                       "Theppitak Karoonboonyanan <thep@linux.thai.net>",
                                                       PKGDATADIR"/icons/ibus-lanxang-tham.svg",
                                                       "us"));
      ibus_component_add_engine (component,
                                 ibus_engine_desc_new ("lanxang-tn",
                                                       "LanXang Thai Noi",
                                                       "Input method for Thai Noi script",
                                                       "lo",
                                                       "GPL",
                                                       "Theppitak Karoonboonyanan <thep@linux.thai.net>",
                                                       PKGDATADIR"/icons/ibus-lanxang-tn.svg",
                                                       "us"));
      ibus_component_add_engine (component,
                                 ibus_engine_desc_new ("lanxang-pali",
                                                       "LanXang Lao-Pali",
                                                       "Input method for Lao script with Pali extension",
                                                       "lo",
                                                       "GPL",
                                                       "Theppitak Karoonboonyanan <theppitak@gmail.com>",
                                                       PKGDATADIR"/icons/ibus-lanxang-pali.svg",
                                                       "us"));
    }

  return component;
}

/*
vi:ts=2:nowrap:ai:expandtab
*/
