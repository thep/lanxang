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

#include <libintl.h>
#include "dlg-setup.h"

#define _(string) gettext(string)

#define WIDGET_MARGINS 12

GtkWidget *
ibus_lanxang_setup_dialog_new ()
{
  GtkWidget *main_dlg;
  GtkWidget *main_vbox;
  GtkWidget *widget;
  GtkWidget *vbox;
  char      *markup;
  GtkRadioButton *last_radio;

  /* Create main window */
  main_dlg = gtk_dialog_new_with_buttons (_("IBus LanXang Preferences"),
                                          NULL, GTK_DIALOG_MODAL,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OK, GTK_RESPONSE_OK,
                                          NULL);
  /* Get the top level content vbox */
  main_vbox = gtk_dialog_get_content_area (GTK_DIALOG (main_dlg));
  gtk_widget_set_margin_top (GTK_WIDGET (main_vbox), WIDGET_MARGINS);
  gtk_widget_set_margin_left (GTK_WIDGET (main_vbox), WIDGET_MARGINS);

  /* Input Sequence Check section label */
  widget = gtk_label_new (NULL);
  markup = g_markup_printf_escaped ("<b>%s</b>",
                                    _("Input sequence check mode:"));
  gtk_label_set_markup (GTK_LABEL (widget), markup);
  g_free (markup);
  gtk_misc_set_alignment (GTK_MISC (widget), 0, 0.5);
  gtk_widget_set_margin_right (widget, WIDGET_MARGINS);
  gtk_widget_show (widget);
  gtk_box_pack_start (GTK_BOX (main_vbox), widget, TRUE, FALSE, 0);

  /* Input Sequence Check vbox for radio buttons */
  vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_margin_left (vbox, WIDGET_MARGINS);
  gtk_widget_set_margin_right (vbox, WIDGET_MARGINS);
  gtk_widget_show (vbox);
  gtk_box_pack_start (GTK_BOX (main_vbox), vbox, TRUE, TRUE, 0);

  /* - "No Check" choice */
  widget = gtk_radio_button_new_with_mnemonic (NULL, _("_No Check"));
  gtk_widget_set_tooltip_text (
    widget,
    _("Let all key sequences pass through.")
  );
  gtk_widget_show (widget);
  gtk_box_pack_start (GTK_BOX (vbox), widget, TRUE, FALSE, 0);
  last_radio = GTK_RADIO_BUTTON (widget);

  g_object_set_data (G_OBJECT (main_dlg), "no_check_radio", widget);

  /* - "Basic Check" choice */
  widget = gtk_radio_button_new_with_mnemonic_from_widget (last_radio,
                                                           _("_Basic Check"));
  gtk_widget_set_tooltip_text (
    widget,
    _("Check using basic rules, allowing more spelling exceptions.")
  );
  gtk_widget_show (widget);
  gtk_box_pack_start (GTK_BOX (vbox), widget, TRUE, FALSE, 0);
  last_radio = GTK_RADIO_BUTTON (widget);

  g_object_set_data (G_OBJECT (main_dlg), "basic_check_radio", widget);

  /* - "Strict Check" choice */
  widget = gtk_radio_button_new_with_mnemonic_from_widget (last_radio,
                                                           _("_Strict Check"));
  gtk_widget_set_tooltip_text (
    widget,
    _("Check using more strict orthographical rules, "
      "disallowing many exeptions.")
  );
  gtk_widget_show (widget);
  gtk_box_pack_start (GTK_BOX (vbox), widget, TRUE, FALSE, 0);

  g_object_set_data (G_OBJECT (main_dlg), "strict_check_radio", widget);

  return main_dlg;
}

void
ibus_lanxang_setup_set_values (const GtkDialog *dlg,
                               const IBusLanXangSetupOptions *opt)
{
  GtkWidget *widget;

  /* Set input sequence check value */
  switch (opt->isc_mode)
    {
      case ISC_PASSTHROUGH:
        widget = g_object_get_data (G_OBJECT (dlg), "no_check_radio");
        break;
      case ISC_STRICT:
        widget = g_object_get_data (G_OBJECT (dlg), "strict_check_radio");
        break;
      case ISC_BASIC:
      default:
        widget = g_object_get_data (G_OBJECT (dlg), "basic_check_radio");
        break;
    }
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (widget), TRUE);
}

void
ibus_lanxang_setup_get_values (const GtkDialog *dlg,
                               IBusLanXangSetupOptions *opt)
{
  GtkWidget *widget;

  /* Check for input sequence check value */
  widget = g_object_get_data (G_OBJECT (dlg), "no_check_radio");
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (widget)))
    {
      opt->isc_mode = ISC_PASSTHROUGH;
      goto isc_mode_done;
    }
  widget = g_object_get_data (G_OBJECT (dlg), "strict_check_radio");
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (widget)))
    {
      opt->isc_mode = ISC_STRICT;
      goto isc_mode_done;
    }
  opt->isc_mode = ISC_BASIC;

isc_mode_done:
  return;
}

/*
vi:ts=2:nowrap:ai:expandtab
*/
