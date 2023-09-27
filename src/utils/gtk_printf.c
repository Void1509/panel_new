#include <stdio.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>

void lbl_printf(GtkLabel *lbl, gchar *fmt, ...) {
	gchar buf[256];
	va_list pt;
	va_start(pt, fmt);
	g_vsprintf(buf, fmt, pt);
	va_end(pt);
	gtk_label_set_text(lbl, buf);
}

void ent_printf(GtkEntry *ent, gchar *fmt, ...) {
	gchar buf[256];
	va_list pt;
	va_start(pt, fmt);
	g_vsprintf(buf, fmt, pt);
	va_end(pt);
	gtk_entry_set_text(ent, buf);
}


