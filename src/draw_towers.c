#include <stdio.h>
#include <gtk/gtk.h>
#include <cairo.h>
#include "pango/pango-attributes.h"
#include "settings.h"
#include "mcu_data.h"

struct Tower_s {
	uint8_t inx, len;
	GtkBox *box;
	GtkLabel *lbl;
	GtkScrolledWindow *scr;
	GtkViewport *vp;
	GtkDrawingArea *draw;
};

static struct Towers_s {
	GtkBox *box;
	struct Tower_s tower[16];
}towers;

gboolean tower_draw_cb(GtkWidget *wg, cairo_t *ctx, gpointer p) {
	struct Tower_s *twr = p;
	int w,h;
	double xd, yd, wd, hd, hdb;

	w = gtk_widget_get_allocated_width(wg);
	h = gtk_widget_get_allocated_height(wg);
	wd = ((float)w / 3.f) * 2.f;
	hd = h - 10;
	hdb = hd / (float)twr->len;

	xd = ((double)w / 2.f) - (wd /2);
	yd = 5;

	cairo_set_source_rgb(ctx, 0.302, 0.467, 0.678);
	cairo_paint(ctx);

	cairo_set_source_rgb(ctx, 0.494, 0.839, 0.494);
	cairo_rectangle(ctx, xd, yd, wd, hd);
	cairo_fill(ctx);
	cairo_set_source_rgb(ctx, 0., 0.567, 0.);
	cairo_rectangle(ctx, xd, yd, wd, hd);
	cairo_stroke(ctx);
	
	for(int i = 1; i < twr->len; i++) {
		cairo_move_to(ctx, xd, yd + (hdb * (float)i));
		cairo_line_to(ctx, xd + wd, yd + (hdb * (float)i));
		cairo_stroke(ctx);
	}
	return TRUE;
}

static GtkWidget* build_tower(int inx, int len) {
	struct Tower_s twr;
	char str[6];

	len &= 0xf;

	twr.box = (GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
	gtk_widget_set_vexpand_set((GtkWidget*)twr.box, TRUE);
	snprintf(str, 6, "%d", inx + 1);
	twr.lbl = (GtkLabel*)gtk_label_new(str);
	gtk_label_set_attributes(twr.lbl, pango_attr_list_from_string("0 -1 size 20000"));
	gtk_box_pack_start(twr.box, (GtkWidget*)twr.lbl, FALSE, TRUE, 2);

	twr.draw = (GtkDrawingArea*)gtk_drawing_area_new();
	g_object_set(twr.draw, "height-request", 50 * len, NULL);
	g_signal_connect(twr.draw, "draw", G_CALLBACK(tower_draw_cb), &towers.tower[inx]);
	twr.vp = (GtkViewport*)gtk_viewport_new(NULL, NULL);
	gtk_container_add((GtkContainer*)twr.vp, (GtkWidget*)twr.draw);
	twr.scr =(GtkScrolledWindow*)gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_min_content_width(twr.scr, 120);
	gtk_scrolled_window_set_max_content_width(twr.scr, 200);
//	gtk_widget_set_hexpand((GtkWidget*)towers.tower[i].box, TRUE);
	gtk_container_add((GtkContainer*)twr.scr, (GtkWidget*)twr.vp);
	gtk_box_pack_start(twr.box, (GtkWidget*)twr.scr, TRUE, TRUE, 1);
	twr.inx = inx;
	twr.len = len;
	towers.tower[inx] = twr;
	return (GtkWidget*)twr.box;
}

GtkWidget *towers_init() {

	towers.box = (GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_widget_set_name((GtkWidget*)towers.box, "towers");
//	g_object_set(towers.box, "height-request", 450, NULL);
	gtk_widget_set_vexpand((GtkWidget*)towers.box, TRUE);
	for(int i = 0; i < appSettings.TowersCount; i++) {

		gtk_box_pack_start(towers.box, build_tower(i, appSettings.SemensCount[i]), TRUE, TRUE, 1);
	}

	return(GTK_WIDGET(towers.box));
}

static gboolean draw_tower_cb(GtkWidget *w, cairo_t *ctx, gpointer p) {
	int width = gtk_widget_get_allocated_width(w);
	int height = gtk_widget_get_allocated_height(w);
	int tow = 1;
}


