// SPDX-License-Identifier: GPL-2.0-only
/*
 * callbacks.c for USBView - a USB device viewer
 * Copyright (c) 1999, 2000 by Greg Kroah-Hartman, <greg@kroah.com>
 */
#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include <gtk/gtk.h>
#include "usbtree.h"


void on_buttonClose_clicked (GtkButton *button, gpointer user_data)
{
	if (mainloop && g_main_loop_is_running(mainloop))
		g_main_loop_quit(mainloop);
}


gboolean on_window1_delete_event (GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
	if (mainloop && g_main_loop_is_running(mainloop))
		g_main_loop_quit(mainloop);
	return FALSE;
}


void on_buttonRefresh_clicked (GtkButton *button, gpointer user_data)
{
	LoadUSBTree(1);
}


void on_buttonAbout_clicked (GtkButton *button, gpointer user_data)
{
	GdkPixbuf *pixbuf;
	GdkTexture *logo;
	gchar *authors[] = { "Greg Kroah-Hartman <greg@kroah.com>", NULL };
	GError *error = NULL;
	gchar *logo_path = NULL;
	const gchar *possible_paths[] = {
		"usbview_logo.svg",                      // Current directory (for development)
		"/usr/share/usbview/usbview_logo.svg",   // Installed location
		"/usr/local/share/usbview/usbview_logo.svg", // Local install
		NULL
	};

	// Try to find the SVG file in various locations
	for (int i = 0; possible_paths[i] != NULL; i++) {
		if (g_file_test(possible_paths[i], G_FILE_TEST_EXISTS)) {
			logo_path = g_strdup(possible_paths[i]);
			break;
		}
	}

	if (!logo_path) {
		g_warning("Could not find usbview_logo.svg in any expected location");
		// Fallback to no logo
		gtk_show_about_dialog (GTK_WINDOW (windowMain),
			"program-name", "usbview",
			"version", VERSION,
			"comments", "Display information on USB devices",
			"website-label", "http://www.kroah.com/linux-usb/",
			"website", "http://www.kroah.com/linux-usb/",
			"copyright", "Copyright © 1999-2012, 2021-2022",
			"authors", authors,
			NULL);
		return;
	}

	// Load SVG and scale it to 2x size for better visibility (664x472)
	pixbuf = gdk_pixbuf_new_from_file_at_size(logo_path, 664, 472, &error);
	g_free(logo_path);
	if (!pixbuf) {
		g_warning("Failed to load logo SVG: %s", error ? error->message : "Unknown error");
		if (error) g_error_free(error);
		// Fallback to no logo
		gtk_show_about_dialog (GTK_WINDOW (windowMain),
			"program-name", "usbview",
			"version", VERSION,
			"comments", "Display information on USB devices",
			"website-label", "http://www.kroah.com/linux-usb/",
			"website", "http://www.kroah.com/linux-usb/",
			"copyright", "Copyright © 1999-2012, 2021-2022",
			"authors", authors,
			NULL);
		return;
	}
	
	logo = gdk_texture_new_for_pixbuf (pixbuf);
	gtk_show_about_dialog (GTK_WINDOW (windowMain),
		"logo", logo,
		"program-name", "usbview",
		"version", VERSION,
		"comments", "Display information on USB devices",
		"website-label", "http://www.kroah.com/linux-usb/",
		"website", "http://www.kroah.com/linux-usb/",
		"copyright", "Copyright © 1999-2012, 2021-2022",
		"authors", authors,
		NULL);
	g_object_unref (pixbuf);
	g_object_unref (logo);
}


gint on_timer_timeout (gpointer user_data)
{
	LoadUSBTree(0);
	return 1;
}

